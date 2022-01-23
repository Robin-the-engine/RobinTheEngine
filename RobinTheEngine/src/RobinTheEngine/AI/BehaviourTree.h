#pragma once
#include <string>
#include <vector>
#include <memory>

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
    virtual void abort(TreeState& treePath);
    virtual void onInit() {}
    virtual void onFinish() {}
};

class BehaviourTree {
public:
    BehaviourTree(const std::string& path);
    TickResult tick(TreeState& state);
private:
    void buildTree(const std::string& path);
    std::shared_ptr<Behaviour> head {nullptr};
};

class SeqBehaviour: public Behaviour {
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
    explicit Parallel(Policy p);
    TickResult tick(TreeState& treePath) override;
    void abort(TreeState& treePath) override;
private:
    Policy policy;
};

class Action final: public Behaviour {
public:
    TickResult tick(TreeState& treePath) override;
    void abort(TreeState& treePath) override;
};

class Condition final: public Behaviour {
public:
    TickResult tick(TreeState& treePath) override;
};
