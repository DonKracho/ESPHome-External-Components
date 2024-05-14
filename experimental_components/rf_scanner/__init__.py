import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import pins
from esphome.const import CONF_ID

MULTI_CONF = True

CONF_DATAPIN = "data_pin"
CONF_SCANPIN = "scan_pin"

rf_scanner_ns = cg.esphome_ns.namespace('rf_scanner')
RfScanner = rf_scanner_ns.class_("RfScanner", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RfScanner),
        cv.Required(CONF_DATAPIN): pins.gpio_input_pin_schema,
        cv.Optional(CONF_SCANPIN): pins.gpio_input_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_DATAPIN])
    cg.add(var.set_datapin(pin))

    if CONF_SCANPIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_SCANPIN])
        cg.add(var.set_scanpin(pin))
