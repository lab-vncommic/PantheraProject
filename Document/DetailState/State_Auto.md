```mermaid
sequenceDiagram
    participant Main as Main
    participant App as Panthera
    participant State as StateManagement
    participant Meas as Measurement
    participant Fault as FaultManagement
    participant Btn as Button
    participant Comm as Communication
    participant Mem as Memory
    participant Motor as MotorDriver

    Main->>App: PIT0_ISR()
    App->>State: PTR_main_interrupt_service()
    State->>Meas: STATE_UpdateMeas()
    Meas-->>State: Measurement values

    State->>Fault: STATE_UpdateFault()
    Fault-->>State: Fault status

    State->>Btn: STATE_UpdateButton()
    Btn-->>State: Button states

    State->>Comm: STATE_ProcessInputSignals()
    Comm-->>State: Communication status

    State->>State: STATE_UpdateState()

    %% AUTO state logic
    State->>Mem: MEM_GetSeatAxis(axis, n)
    Mem-->>State: Memory axis n

    State->>Motor: MD_Run() (move to target axis)
    Motor-->>State:

    alt cnt_use_recall >= 10
        State-->>State: Change to HOMING state
    else All axes reached target (flag_auto_completed == true)
        State->>Motor: MD_Run(STOP)
        Motor-->>State:
        State-->>State: Change to IDLE state
    else Fault detected (fault.mode != FAULT_NONE)
        State-->>State: Change to FAULT state
    else Remain in AUTO
    end

    State-->>App: PTR_main_interrupt_service() completed
    App-->>Main: Interrupt service completed
```