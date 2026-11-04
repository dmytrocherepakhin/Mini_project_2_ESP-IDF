
# Button State Machines

Three FSM variants for four ESP32-S3 buttons.

- SW1, SW4 (GPIO16): Short + Long Press
- SW2: Short Press
- SW3: Short + Double Press

Parameters:
- Debounce: 50 ms
- Long press: 2000 ms
- Double press window: 300 ms

```mermaid

stateDiagram
  direction TB
  state S2 {
    direction TB
    [*] --> S2_Released
    S2_Released --> S2_DbPress:LOW
    S2_DbPress --> S2_Pressed:50ms / LOW
    S2_DbPress --> S2_Released:50ms / HIGH
    S2_Pressed --> S2_DbRelease:HIGH
    S2_DbRelease --> S2_Pressed:50ms / LOW
    S2_DbRelease --> S2_Released:50ms / HIGH, SHORT
  }
  state S1_4 {
    direction TB
    [*] --> S1_4_Released
    S1_4_Released --> S1_4_DbPress:LOW
    S1_4_DbPress --> S1_4_Pressed:50ms / LOW
    S1_4_DbPress --> S1_4_Released:50ms / HIGH
    S1_4_Pressed --> S1_4_Long:2000ms / LONG
    S1_4_Pressed --> S1_4_DbRelease:HIGH
    S1_4_DbRelease --> S1_4_Pressed:50ms / LOW
    S1_4_DbRelease --> S1_4_Released:50ms / HIGH, SHORT
    S1_4_Long --> S1_4_DbLongRelease:HIGH
    S1_4_DbLongRelease --> S1_4_Long:50ms / LOW
    S1_4_DbLongRelease --> S1_4_Released:50ms / HIGH
  }
  state S3 {
    direction TB
    [*] --> S3_Released
    S3_Released --> S3_DbPress1:LOW
    S3_DbPress1 --> S3_Pressed1:50ms / LOW
    S3_DbPress1 --> S3_Released:50ms / HIGH
    S3_Pressed1 --> S3_DbRelease1:HIGH
    S3_DbRelease1 --> S3_Pressed1:50ms / LOW
    S3_DbRelease1 --> S3_Wait2:50ms / HIGH
    S3_Wait2 --> S3_Released:300ms / SHORT
    S3_Wait2 --> S3_DbPress2:LOW before deadline
    S3_DbPress2 --> S3_Pressed2:50ms / LOW
    S3_DbPress2 --> S3_Wait2:HIGH, window active
    S3_DbPress2 --> S3_Released:HIGH, expired / SHORT
    S3_Pressed2 --> S3_DbRelease2:HIGH
    S3_DbRelease2 --> S3_Pressed2:50ms / LOW
    S3_DbRelease2 --> S3_Released:50ms / HIGH, DOUBLE
  }
  S2:SW2 - SHORT
  S1_4:SW1 / SW4 - SHORT + LONG
  S3:SW3 - SHORT + DOUBLE
  ```
