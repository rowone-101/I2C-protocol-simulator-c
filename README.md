# I2C Protocol Simulator in C

A software simulation of the **I2C (Inter-Integrated Circuit)** communication protocol, written in C. Built to understand embedded communication protocols at the bit level without access to physical hardware.

## What This Project Does

This project models how a Master device communicates with a Slave device over I2C — including addressing, data transfer, and acknowledgment — entirely in software. It prints a bit-by-bit trace of the simulated SDA (data) line so you can see exactly how a real I2C transaction is structured.

## Files

**`i2c_simulator.c`** — Simulation of an I2C write and read transaction between one master and one slave.

## Concepts Demonstrated

- START and REPEATED START / STOP conditions
- 7-bit slave addressing + Read/Write bit
- Byte framing and ACK/NACK handshaking
- Register-based read/write to a simulated slave device

## How to Run

```bash
gcc i2c_simulator.c -o i2c_simulator
./i2c_simulator
```

This is a plain C program — no external libraries or hardware required.

## Sample Output

```
I2C WRITE TRANSACTION
START condition (SDA falls while SCL is high)
Sending byte [Slave Address + Write bit]: 0xA0
...
ACK/NACK   : 0
STOP condition (SDA rises while SCL is high)

Verification: Read back value = 0x7A
Write/Read cycle SUCCESSFUL
```

## Note

This is a **software simulation**, not firmware running on real hardware. It was built to learn and demonstrate understanding of the I2C protocol's structure and logic, since physical development boards weren't available. No physical SDA/SCL lines are involved — the bit-level behavior is modeled purely through code.

## Author

Ruhan Paul
