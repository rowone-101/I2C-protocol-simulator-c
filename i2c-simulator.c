/*
 * I2C Master-Slave Communication Protocol Simulator
 * --------------------------------------------------
 * This program simulates the I2C (Inter-Integrated Circuit) protocol
 * at the bit level in software, without any physical hardware.
 *
 * It demonstrates:
 *   - START and STOP conditions
 *   - 7-bit slave addressing + Read/Write bit
 *   - Byte framing (8 data bits + 1 ACK/NACK bit)
 *   - Master writing data to a simulated slave register
 *   - Master reading data back from the slave
 *   - A printed "timing diagram" showing SDA line bit-by-bit
 *
 * Author: <Your Name>
 * Purpose: Educational simulation of embedded communication protocol
 */

#include <stdio.h>
#include <stdint.h>

#define NUM_SLAVE_REGISTERS 8

/* ---------- Simulated Slave Device ---------- */
typedef struct {
    uint8_t address;                          /* 7-bit slave address     */
    uint8_t registers[NUM_SLAVE_REGISTERS];   /* internal register file  */
} I2C_Slave;

/* ---------- Helper: print a single bit on the "line" ---------- */
void print_bit(const char *label, int bit) {
    printf("%-10s : %d\n", label, bit);
}

/* ---------- Simulate sending one byte + reading ACK/NACK ---------- */
int send_byte(uint8_t byte, const char *context) {
    printf("\n-- Sending byte in [%s]: 0x%02X --\n", context, byte);
    for (int i = 7; i >= 0; i--) {
        int bit = (byte >> i) & 0x01;
        printf("  bit%d (SDA) = %d\n", i, bit);
    }
    /* In real I2C, the receiver pulls SDA low for ACK (0) after 8 bits.
       We simulate a successful transfer -> ACK = 0 */
    int ack = 0;
    print_bit("ACK/NACK", ack);
    return ack; /* 0 = ACK, 1 = NACK */
}

/* ---------- Simulate receiving one byte from slave ---------- */
uint8_t receive_byte(uint8_t data, const char *context) {
    printf("\n-- Receiving byte in [%s]: 0x%02X --\n", context, data);
    for (int i = 7; i >= 0; i--) {
        int bit = (data >> i) & 0x01;
        printf("  bit%d (SDA) = %d\n", i, bit);
    }
    /* Master sends ACK to continue, or NACK to stop -- here we ACK */
    print_bit("Master ACK", 0);
    return data;
}

/* ---------- I2C Write: Master -> Slave ---------- */
void i2c_write(I2C_Slave *slave, uint8_t reg_addr, uint8_t data) {
    printf("\n=========================================\n");
    printf("I2C WRITE TRANSACTION\n");
    printf("=========================================\n");

    printf("START condition (SDA falls while SCL is high)\n");

    /* Address byte: 7-bit address + R/W bit (0 = write) */
    uint8_t addr_byte = (slave->address << 1) | 0;
    send_byte(addr_byte, "Slave Address + Write bit");

    /* Register pointer byte */
    send_byte(reg_addr, "Register Address");

    /* Data byte */
    send_byte(data, "Data");

    printf("STOP condition (SDA rises while SCL is high)\n");

    /* Commit to simulated slave memory */
    if (reg_addr < NUM_SLAVE_REGISTERS) {
        slave->registers[reg_addr] = data;
        printf("\n[Slave] Register 0x%02X updated to 0x%02X\n", reg_addr, data);
    }
}

/* ---------- I2C Read: Master <- Slave ---------- */
uint8_t i2c_read(I2C_Slave *slave, uint8_t reg_addr) {
    printf("\n=========================================\n");
    printf("I2C READ TRANSACTION\n");
    printf("=========================================\n");

    printf("START condition\n");

    /* Address byte with Write bit to set register pointer first */
    uint8_t addr_byte_w = (slave->address << 1) | 0;
    send_byte(addr_byte_w, "Slave Address + Write bit");
    send_byte(reg_addr, "Register Address to read");

    printf("REPEATED START condition\n");

    /* Address byte with Read bit */
    uint8_t addr_byte_r = (slave->address << 1) | 1;
    send_byte(addr_byte_r, "Slave Address + Read bit");

    uint8_t value = (reg_addr < NUM_SLAVE_REGISTERS) ? slave->registers[reg_addr] : 0xFF;
    receive_byte(value, "Register Data");

    printf("STOP condition\n");

    return value;
}

int main(void) {
    /* Create a simulated slave device at address 0x50 */
    I2C_Slave temp_sensor = {0};
    temp_sensor.address = 0x50;

    printf("############################################\n");
    printf("  I2C PROTOCOL SIMULATION (Software Only)\n");
    printf("  Simulated Slave Address: 0x%02X\n", temp_sensor.address);
    printf("############################################\n");

    /* Master writes 0x7A into register 0x02 of the slave */
    i2c_write(&temp_sensor, 0x02, 0x7A);

    /* Master reads back register 0x02 to verify */
    uint8_t result = i2c_read(&temp_sensor, 0x02);

    printf("\n=========================================\n");
    printf("Verification: Read back value = 0x%02X\n", result);
    printf("Write/Read cycle %s\n",
           (result == 0x7A) ? "SUCCESSFUL" : "FAILED");
    printf("=========================================\n");

    return 0;
}
