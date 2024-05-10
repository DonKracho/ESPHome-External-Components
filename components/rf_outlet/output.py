import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import output
from esphome.const import CONF_ID, CONF_PIN

CODEOWNERS = ["@glmnet"]

rf_outlet_ns = cg.esphome_ns.namespace("rf_outlet")
RfOutlet = rf_outlet_ns.class_("RfOutlet", output.FloatOutput, cg.Component)

CONF_REPEAT = "repeat"

CONFIG_SCHEMA = cv.All(
    output.FLOAT_OUTPUT_SCHEMA.extend(
        {
            cv.Required(CONF_ID): cv.declare_id(RfOutlet),
            cv.Required(CONF_PIN): pins.internal_gpio_output_pin_schema,
            cv.Optional(CONF_REPEAT, default=6): cv.All(cv.uint16_t, cv.Range(min=0, max=10)),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.only_with_arduino,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_tx_pin(pin))

    cg.add(var.set_repeat(config[CONF_REPEAT]))