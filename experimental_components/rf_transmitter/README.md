```yaml
# example configuration:

rf_transmitter:
  pin: GPIO2
  repeat: 6
 
switch:
  - platform: template
    name: "Funk 00"
    turn_on_action:
      rf_transmitter.turn_on:
        channel: 0
    turn_off_action:
      rf_transmitter.turn_off:
        channel: 0
```