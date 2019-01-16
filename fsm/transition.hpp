//
// Created by ocanty on 16/01/19.
//

#ifndef MIPS_ASM_TRANSITION_HPP
#define MIPS_ASM_TRANSITION_HPP

#include <functional>

namespace as {

template <typename States, typename InputsType>
class transition {
public:
    /**
     * Defines a transition between one state to another
     *
     * @param leaving_state             The state we can leave
     * @param entering_state            The state we will enter
     * @param should_transition_func    A function that returns true if we should transition, the input set
     *                                  is passed as the first parameter
     * @param transition_callback_func  A function that will be called when the transition occurs
     */
    transition(
        const States& leaving_state,
        const States& entering_state,
        std::function<bool(const InputsType&)> should_transition_func,
        std::function<void(const InputsType&)> transition_callback_func
    ) :
        m_start_state(leaving_state),
        m_finish_state(entering_state),
        m_should_transition_func(should_transition_func),
        m_transition_callback_func(transition_callback_func)
    {

    }

    /**
     * Defines a transition between one state to another
     *
     * @param leaving_state             The state we can leave
     * @param entering_state            The state we will enter
     * @param should_transition_func    A function that returns true if we should transition, the input set
     *                                  is passed as the first parameter
     */
     transition(
        const States& leaving_state,
        const States& entering_state,
        std::function<bool(const InputsType&)> should_transition_func
    ) :
        m_start_state(leaving_state),
        m_finish_state(entering_state),
        m_should_transition_func(should_transition_func),
        m_transition_callback_func([](){})
    {

    }

    /**
     * Virtual destructor, for inheritance
     */
    virtual ~transition() = default;

    /**
     * @return The initial state required for the transition
     */
    States get_initial_state() const {
        return m_start_state;
    }

    /**
     * @return The state after the transition
     */
    States get_transition_state() const {
        return m_finish_state;
    }

    /**
     * Tests if the input passed will trigger the transition
     * @param inputs The input set
     * @return true if the input will trigger the transition, false otherwise
     */
    bool test_transition_condition(const InputsType& inputs) const {
        return m_should_transition_func(inputs);
    }

    std::function<void(const InputsType&)> get_transition_callback() const {
        return m_transition_callback_func;
    }

private:
    /**
     * The current state
     */
    States m_start_state;

    /**
     * The state we will move to
     */
    States m_finish_state;

    /**
     * A function that returns true when the value of the input type warrants
     * the current state to transition to a new one
     */
    std::function<bool(const InputsType&)> m_should_transition_func;

    /**
     * A function that will be called when the state transitions
     */
    std::function<void(const InputsType&)> m_transition_callback_func;
};

}

#endif //MIPS_ASM_TRANSITION_HPP
