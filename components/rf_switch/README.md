```yaml
# example configuration:

switch:
  - platform: rf_switch
    name: 'rf switch'
	output: rf_transmitter
	channel: 0

output:
  - platform: rf_outlet
	id: rf_transmitter
    pin: GPIO2
	repeat: 3
```