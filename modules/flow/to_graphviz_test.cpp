#include <iostream>
#include <gtest/gtest.h>
#include <tbox/base/json.hpp>
#include <tbox/event/loop.h>
#include <tbox/base/scope_exit.hpp>

#include "to_graphviz.h"
#include "actions/function_action.h"
#include "actions/sequence_action.h"
#include "actions/if_else_action.h"
#include "actions/repeat_action.h"
#include "actions/succ_fail_action.h"

#include "state_machine.h"

namespace tbox {
namespace flow {

TEST(ToGraphviz, ActionJson) {
    auto loop = event::Loop::New();
    SetScopeExitAction([loop] { delete loop; });

    auto seq_action = new SequenceAction(*loop);
    seq_action->set_label("This is test");
    auto if_else_action = new IfElseAction(*loop,
            new SuccAction(*loop),
            new FunctionAction(*loop, []{return true;}),
            new FunctionAction(*loop, []{return true;}));
    if_else_action->set_label("Just If-Else");
    seq_action->append(if_else_action);
    auto repeat_action = new RepeatAction(*loop,
            new FunctionAction(*loop, []{return true;}), 5);
    seq_action->append(repeat_action);
    seq_action->start();

    std::cout << ToGraphviz(seq_action);

    loop->exitLoop(std::chrono::milliseconds(10));
    loop->runLoop();

    delete seq_action;
}

TEST(ToGraphviz, StateMachineJson)
{
    using SM = StateMachine;

    enum class StateId { kTerm, kInit, k1, k2 };
    enum class EventId { kTerm, k1, k2, k3 };

    SM sm, sub_sm;

    sm.newState(StateId::kInit, nullptr, nullptr, "Init");
    sm.newState(StateId::k1,    nullptr, nullptr, "State1");
    sm.newState(StateId::k2,    nullptr, nullptr, "State2");

    sm.addRoute(StateId::kInit, EventId::k1, StateId::k1,    nullptr, nullptr, "Event1");
    sm.addRoute(StateId::kInit, EventId::k2, StateId::k2,    nullptr, nullptr, "Event2");
    sm.addRoute(StateId::k1,    EventId::k1, StateId::kTerm, nullptr, nullptr, "Event1");
    sm.addRoute(StateId::k1,    EventId::k2, StateId::k2,    nullptr, nullptr, "Event2");
    sm.addRoute(StateId::k2,    EventId::k1, StateId::k1,    nullptr, nullptr, "Event1");
    sm.addRoute(StateId::k2,    EventId::k2, StateId::k1,    nullptr, nullptr, "Event2");
    sm.addEvent(StateId::k2,    EventId::k3, [](Event) { return -1; });

    sub_sm.newState(StateId::kInit, nullptr, nullptr, "Init");
    sub_sm.newState(StateId::k1,    nullptr, nullptr, "SubState1");
    sub_sm.newState(StateId::k2,    nullptr, nullptr, "SubState2");

    sub_sm.addRoute(StateId::kInit, EventId::k1, StateId::k1,    nullptr, nullptr, "Event1");
    sub_sm.addRoute(StateId::kInit, EventId::k2, StateId::k2,    nullptr, nullptr, "Event2");
    sub_sm.addRoute(StateId::k1,    EventId::k2, StateId::k2,    nullptr, nullptr, "Event2");
    sub_sm.addRoute(StateId::k1,    EventId::k1, StateId::kTerm, nullptr, nullptr, "Event1");
    sub_sm.addRoute(StateId::k2,    EventId::k1, StateId::k1,    nullptr, nullptr, "Event1");
    sub_sm.addRoute(StateId::k2,    EventId::k2, StateId::kTerm, nullptr, nullptr, "Event2");

    sm.setSubStateMachine(StateId::k1, &sub_sm);
    sm.start();
    sm.run(EventId::k1);

    std::cout << ToGraphviz(sm);
}

}
}
