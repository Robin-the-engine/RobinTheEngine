#include <vector>
#include <condition_variable>
#include <Windows.h>

#include <VGJS.h>
#include <VGJSCoro.h>

#include "JobPriority.h"

namespace RTE {

    struct JobHandle {
        friend class JobSystem;
    private:
        JobHandle(vgjs::Job* job, uint64_t job_id) : job(job), job_id(job_id){}
        vgjs::Job* job = nullptr;
        uint64_t job_id = 0;
    };

    template<typename F>
    concept VGJS_JOB = vgjs::FUNCTOR<F>;

    template<VGJS_JOB F>
    struct JobDescription {

        JobDescription(
            F&& function,
            JobPriority priority = JobPriority::HIGH,
            size_t threadNumber = -1,	// -1 means any thread
            std::vector<JobHandle> *waitJobs = nullptr // which job should be done before start this job
        ) : function(std::forward<decltype(function)>(function)), priority(priority), threadNumber(threadNumber), waitJobs(waitJobs) {};

        F&& function;
        JobPriority priority;
        size_t threadNumber;
        std::vector<JobHandle> *waitJobs;
    };

    class JobSystem final {

    public:
        ~JobSystem() {
            vgjs::wait_for_termination();
        }

        template<VGJS_JOB F>
        JobHandle kickJob(const JobDescription<F>& desc) {
            if (desc.waitJobs) {
                waitForJobs(*desc.waitJobs);
            }
            std::pair <vgjs::Job*, uint64_t > scheduled = jobSystem.schedule(desc.function, desc.priority, vgjs::thread_index_t(desc.threadNumber));
            return JobHandle{ scheduled.first, scheduled.second};
        }

        template<VGJS_JOB F>
        JobHandle kickJob(
            F&& function,
            std::vector<JobHandle> &waitJobs,
            JobPriority priority = JobPriority::HIGH,
            size_t threadNumber = -1
        ) {
            waitForJobs(waitJobs);
            std::pair<vgjs::Job*, uint64_t> scheduled = jobSystem.schedule(function, priority, vgjs::thread_index_t(threadNumber));
            return JobHandle{ scheduled.first, scheduled.second };
        }

        template<VGJS_JOB F>
        JobHandle kickJob(
            F&& function,
            JobPriority priority = JobPriority::HIGH,
            size_t threadNumber = -1
        ) {
            std::pair<vgjs::Job*, uint64_t> scheduled = jobSystem.schedule(function, priority, vgjs::thread_index_t(threadNumber));
            return JobHandle{ scheduled.first, scheduled.second };
        }

        bool isJobDone(const JobHandle& handle) {
            return handle.job == nullptr || handle.job->m_unique_id != handle.job_id;
        }

        void waitForJob(const JobHandle& handle) {
            if (handle.job) {
                std::unique_lock lg{ handle.job->m_mutex };
                handle.job->m_cv.wait(lg, [&]() { return isJobDone(handle); });
            }
        }

        template<VGJS_JOB F>
        std::vector<JobHandle> kickJobs(const std::vector<JobDescription<F>> &desc) {
            std::vector<JobHandle> handles(desc.size());
            for (auto &jobDesc : desc) {
                handles.emplace_back(kickJob(jobDesc));
            }
            return handles;
        }

        bool isJobsDone(const std::vector<JobHandle> &handles) {
            for (auto& handle : handles) {
                if (!isJobDone(handle)) {
                    return false;
                }
            }
            return true;
        }

        void waitForJobs(const std::vector<JobHandle> &handles) {
            for (auto& handle : handles) {
                waitForJob(handle);
            }
        }

        size_t getThreadCount() const {
            return jobSystem.get_thread_count();
        }

        static JobSystem& GetJobSystem() {
            return js;
        }

    private:
        vgjs::JobSystem jobSystem;
        static JobSystem js;
        JobSystem() : jobSystem(vgjs::thread_count_t(std::thread::hardware_concurrency()), vgjs::thread_index_t(1)) {
            SetThreadAffinityMask(GetCurrentThread(), 0);
        };
    };

}
