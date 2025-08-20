```mermaid
sequenceDiagram
    participant Main as Main
    participant App as Panthera
    participant State as StateManagement
    participant Meas as Measurement
    participant Fault as FaultManagement
    participant Btn as Button
    participant Comm as Communication
    participant Motor as MotorDriver

    Main->>App: PIT0_ISR()
    App->>State: PTR_main_interrupt_service()
    State->>Meas: STATE_UpdateMeas()
    Meas-->>State: Update measurement values
    State->>Fault: STATE_UpdateFault()
    Fault-->>State: Update fault status
    State->>Btn: STATE_UpdateButton()
    Btn-->>State: Update button states
    State->>Comm: STATE_ProcessInputSignals()
    Comm-->>State: Communication status
    State->>State: STATE_UpdateState()

    %% HOMING state logic
    State->>State: STATE_homingHandle()
    State->>Motor: MD_Run() (homing each axis)
    Motor-->>State:
    alt Fault detected (fault.mode != FAULT_NONE)
        State-->>State: Change to FAULT state
    else All axes homed (all block flags true)
        State->>State: STATE_ResetAxis()
        State-->>State: Change to AUTO state
    else Remain in HOMING
    end
    State-->>App: PTR_main_interrupt_service() completed
    App-->>Main: Interrupt service completed
```