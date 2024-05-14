import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_TEMPERATURE,
    CONF_TEMPERATURE,
    UNIT_CELSIUS,
    DEVICE_CLASS_HUMIDITY,
    CONF_HUMIDITY,
    UNIT_PERCENT,
)

from .. import rf_scanner_ns, RfScanner

DEPENDENCIES = ["rf_scanner"]

RfSensor = rf_scanner_ns.class_("RfSensor", sensor.Sensor, cg.PollingComponent,)

CONF_RFSCANNER_ID = "rf_scanner"

CONFIG_SCHEMA = (
    sensor.sensor_schema(RfSensor)
    .extend(
        {
            cv.GenerateID(CONF_RFSCANNER_ID): cv.use_id(RfScanner),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=255),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
               unit_of_measurement=UNIT_CELSIUS,
               accuracy_decimals=1,
               device_class=DEVICE_CLASS_TEMPERATURE,
               state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
               unit_of_measurement=UNIT_PERCENT,
               accuracy_decimals=0,
               device_class=DEVICE_CLASS_HUMIDITY,
               state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_CHANNEL],
    )
    await cg.register_parented(var, config[CONF_RFSCANNER_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
