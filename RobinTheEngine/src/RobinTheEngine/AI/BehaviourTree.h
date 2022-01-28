#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Scene/BaseResource.h"
#include "RobinTheEngine/Scene/Component.h"
#include "TickResult.h"

namespace RTE {

    class Behaviour;

    struct BehaviourState final {
        std::shared_ptr<Behaviour> currentBehaviour = nullptr;
        size_t currentChild = 0;
        bool running = false;
    };

    class AIComponent;

    struct TreeState final {
        TreeState(AIComponent* ai);
        std::vector<BehaviourState> path;
        AIComponent* owner;
    };

    class Behaviour {
    public:
        Behaviour() = default;
        virtual ~Behaviour() = default;
        virtual TickResult tick(TreeState& treeState) = 0;
        virtual void addChild(std::shared_ptr<Behaviour>);
        virtual void abort(TreeState& treeState);
        virtual void onInit() {}
        virtual void onFinish() {}
    };

    struct BehaviourTreeImpl final : public BaseResource {
        BehaviourTreeImpl() = default;
        BehaviourTreeImpl(const std::string& path);
        std::shared_ptr<Behaviour> getHead();
    private:
        void buildTree(const std::string& path);
        std::shared_ptr<Behaviour> head{ nullptr };
    };

    class BehaviourTree final : public Component {
    public:
        BehaviourTree(const std::string& path);
        TickResult tick();
    private:
        BehaviourTreeImpl bti;
    };

    class Action final : public Behaviour {
    public:
        TickResult tick(TreeState& treeState) override;
        void abort(TreeState& treeState) override;
        void setAction(const std::string& description);
    private:
        std::string fname;
        std::string fparams;
    };

    class Condition final : public Behaviour {
    public:
        TickResult tick(TreeState& treeState) override;
        void updateCondition(const std::string& condPart);
    private:
        enum { COND_SIZE = 3 };
        std::array<std::string, COND_SIZE> condition;
        size_t cur_el = 0;
    };

    class SeqBehaviour : public Behaviour {
        void addChild(std::shared_ptr<Behaviour>) override;
    protected:
        std::vector<std::shared_ptr<Behaviour>> childs;
    };

    class Sequence final : public SeqBehaviour {
    public:
        TickResult tick(TreeState& treeState) override;
    };

    class Selector final : public SeqBehaviour {
    public:
        TickResult tick(TreeState& treeState) override;
    };

    class Parallel final : public SeqBehaviour {
    public:
        enum Policy {
            FAILURE_ANY,
            SUCCESS_ANY,
            FAILURE_ALL,
            SUCCESS_ALL,
            REQUIRE_ANY,
            REQUIRE_ALL,
        };
        Parallel() = default;
        TickResult tick(TreeState& treeState) override;
        void abort(TreeState& treeState) override;
        void setPolicy(Policy p);
    private:
        Policy policy = REQUIRE_ANY;
    };

    class Decorator : public Behaviour {
        void addChild(std::shared_ptr<Behaviour>) override;
    protected:
        std::shared_ptr<Behaviour> child;
    };

    class Repeat final : public Decorator {
    public:
        Repeat() = default;
        TickResult tick(TreeState& treeState) override;
        void setTimes(size_t times);
    private:
        std::function<bool(size_t)> repeatCondition = [](size_t) {return true; };
    };

    class Invert final : public Decorator {
    public:
        TickResult tick(TreeState& treeState) override;
    };
}
