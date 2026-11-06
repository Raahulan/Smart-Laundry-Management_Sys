# ThingsBoard Integration

The ESP32 publishes machine state telemetry to ThingsBoard over MQTT and receives RPC commands for remote control.

## MQTT server

```text
mqtt.thingsboard.cloud:1883
```

## Telemetry keys

- `machine1`, `machine2`, `machine3`, `machine4`
- `machine1_available`, `machine2_available`, `machine3_available`, `machine4_available`
- `available_count`

## RPC methods

- `setMachine1`
- `setMachine2`
- `setMachine3`
- `setMachine4`

The dashboard contains four switches so each washing-machine channel can be controlled independently.

## Public repository security

Never publish a real ThingsBoard access token. Keep the token only in your local copy of the firmware or in a secret/credential file excluded by `.gitignore`.
