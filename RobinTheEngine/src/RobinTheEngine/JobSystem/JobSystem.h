#include <VGJS.h>
#include <VGJSCoro.h>
#include "JobPriority.h"
#include <vector>


namespace RTE {

	template<typename F>
	concept VGJS_JOB = vgjs::FUNCTOR<F>;

	template<VGJS_JOB F>
	struct JobDescription {

		JobDescription(
			F&& function,
			JobPriority priority = JobPriority::HIGH,
			size_t threadNumber = -1,	// -1 means any thread
			std::vector<const JobDescription * const> waitJobs = {} // which job should be done before start this job
		) : function(function), priority(priority), threadNumber(threadNumber), waitJobs(waitJobs) {};

		F&& function;
		JobPriority priority;
		size_t threadNumber;
		std::vector<const JobDescription * const> waitJobs;
	};

	class JobSystem final {

	public:

		JobSystem() : jobSystem() {};  // default number of threads is max(2, std::thread::hardware_concurrency())
		JobSystem(size_t threads) : jobSystem(vgjs::thread_count_t(threads)) {};
		~JobSystem() = default;

		template<VGJS_JOB F>
		void kickJob(const JobDescription<F>& desc) {
			waitForJobs(desc.waitJobs);
			jobSystem.schedule(desc.function, desc.priority, desc.threadNumber);
		}

		template<VGJS_JOB F>
		void waitForJob(const JobDescription<F>& desc) {

		}

		template<VGJS_JOB F>
		void kickJobs(const std::vector<const JobDescription<F> * const> desc) {
			for (auto &jobDesc : desc) {
				kickJob(jobDesc);
			}
		}

		template<VGJS_JOB F>
		void waitForJobs(const std::vector<const JobDescription<F> * const> desc) {
			for (auto& jobDesc : desc) {
				waitForJob(jobDesc);
			}
		}

		size_t getThreadCount() const {
			return jobSystem.get_thread_count();
		}

	private:
		vgjs::JobSystem jobSystem;

	};

}