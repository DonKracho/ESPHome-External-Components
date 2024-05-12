import voluptuous as vol

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import switch, output
from esphome.const import CONF_ID, CONF_OUTPUT

rf_switch_ns = cg.esphome_ns.namespace('rf_switch')
RfSwitch = rf_switch_ns.class_('RfSwitch', switch.Switch, cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['switch']

CONF_RFPIN = "rf_pin"
CONF_GROUP = "group"
CONF_CHANNEL = "channel"

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(RfSwitch),
    cv.Required(CONF_OUTPUT): cv.use_id(output.FloatOutput),
    cv.Required(CONF_CHANNEL): vol.All(cv.uint16_t, vol.Range(min=0, max=15)),
    cv.Optional(CONF_GROUP, default=0): vol.All(cv.uint16_t, vol.Range(min=0, max=1)),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    out = await cg.get_variable(config[CONF_OUTPUT])
    cg.add(var.set_output(out))

    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_group(config[CONF_GROUP]))
