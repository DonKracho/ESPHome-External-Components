import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import automation, pins
from esphome.automation import maybe_simple_id
from esphome.const import CONF_ID, CONF_PIN
#from esphome.core import CORE, coroutine_with_priority
#from esphome.cpp_helpers import setup_entity

rf_transmitter_ns = cg.esphome_ns.namespace('rf_transmitter')
RfTransmitter = rf_transmitter_ns.class_("RfTransmitter", cg.Component)

CONF_REPEAT = "repeat"
CONF_CHANNEL = "channel"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RfTransmitter),
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_REPEAT, default=6): cv.All(cv.uint16_t, cv.Range(min=0, max=10)),
}).extend(cv.COMPONENT_SCHEMA)

# actions

TurnOnAction = rf_transmitter_ns.class_("TurnOnAction", automation.Action)
TurnOffAction = rf_transmitter_ns.class_("TurnOffAction", automation.Action)

RF_TRANSMITTER_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(RfTransmitter),
        cv.Required(CONF_CHANNEL): cv.All(cv.uint16_t, cv.Range(min=0, max=15)),
    }
)

@automation.register_action("rf_transmitter.turn_on", TurnOnAction, RF_TRANSMITTER_ACTION_SCHEMA)
@automation.register_action("rf_transmitter.turn_off", TurnOffAction, RF_TRANSMITTER_ACTION_SCHEMA)

async def action_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_rfpin(pin))
    
    cg.add(var.set_repeat(config[CONF_REPEAT]))
