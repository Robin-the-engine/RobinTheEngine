#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Scene/BaseResource.h"

enum class TickResult {
    FAILURE,
    SUCCESS,
    RUNNING,
};

class Behaviour;

struct BehaviourState {
    std::shared_ptr<Behaviour> currentBehaviour = nullptr;
    size_t currentChild = 0;
    bool running = false;
};

struct TreeState {
    std::vector<BehaviourState> path;
};

class Behaviour {
public:
    Behaviour() = default;
    virtual ~Behaviour() = default;
    virtual TickResult tick(TreeState& treePath) = 0;
    virtual void addChild(std::shared_ptr<Behaviour>);
    virtual void abort(TreeState& treePath);
    virtual void onInit() {}
    virtual void onFinish() {}
};

class BehaviourTree final: public RTE::BaseResource {
public:
    BehaviourTree(const std::string& path);
    TickResult tick(TreeState& state);
private:
    void buildTree(const std::string& path);
    std::shared_ptr<Behaviour> head {nullptr};
};

class Action final : public Behaviour {
public:
    TickResult tick(TreeState& treePath) override;
    void abort(TreeState& treePath) override;
};

class Condition final : public Behaviour {
public:
    TickResult tick(TreeState& treePath) override;
    void updateCondition(const std::string& condPart);
private:
    enum {COND_SIZE = 3};
    std::array<std::string, COND_SIZE> condition;
    size_t cur_el = 0;
};

class SeqBehaviour: public Behaviour {
    void addChild(std::shared_ptr<Behaviour>) override;
protected:
    std::vector<std::shared_ptr<Behaviour>> childs;
};

class Sequence final: public SeqBehaviour {
public:
    TickResult tick(TreeState& treePath) override;
};

class Selector final : public SeqBehaviour {
public:
    TickResult tick(TreeState& treePath) override;
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
    TickResult tick(TreeState& treePath) override;
    void abort(TreeState& treePath) override;
    void setPolicy(Policy p);
private:
    Policy policy = REQUIRE_ANY;
};

class Decorator: public Behaviour {
    void addChild(std::shared_ptr<Behaviour>) override;
protected:
    std::shared_ptr<Behaviour> child;
};

class Repeat final: public Decorator {
public:
    Repeat() = default;
    TickResult tick(TreeState& treePath) override;
    void setTimes(size_t times);
private:
    std::function<bool(size_t)> repeatCondition = [](size_t) {return true;};
};

class Invert final : public Decorator {
public:
    TickResult tick(TreeState& treePath) override;
};
