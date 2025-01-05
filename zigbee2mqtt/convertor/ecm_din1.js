const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const ota = require('zigbee-herdsman-converters/lib/ota');
const utils = require('zigbee-herdsman-converters/lib/utils');
const globalStore = require('zigbee-herdsman-converters/lib/store');
const { postfixWithEndpointName, precisionRound } = require('zigbee-herdsman-converters/lib/utils.js') 
const e = exposes.presets;
const ea = exposes.access;

const switchNumberOfDigits = ['0', '1', '2', '3', '4', '5 Digits', '6 Digits'];

const fzLocal = {
  device_number_of_digits: {
    cluster: 'seMetering',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        const result = {};
        const data = msg.data;
        if (data[0xF003] !== undefined) {
            const lookup = {5: '5 Digits', 6: '6 Digits'};
            result.number_of_digits = lookup[data[0xF003]];
        }
        return result;
    },
  },
  device_current_summ_delivered: {
    cluster: 'seMetering',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        const result = {};
        if (msg.data.hasOwnProperty('currentSummDelivered')) {
            const data = msg.data['currentSummDelivered'];
            result.current_summ_delivered = parseInt(data)/100;
		}
        return result;
    },
  },
  device_name: {
    cluster: 'seMetering',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
      const result = {};
      if (msg.data.hasOwnProperty(0xF004)) {
        const data = msg.data[0xF004];
        result.device_name = String(data);
      }
      return result;
    },
  },
  device_divisor: {
    cluster: 'seMetering',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
      const result = {};
      if (msg.data.hasOwnProperty(0xF001)) {
        const data = msg.data[0xF001];
        result.divisor = parseInt(data);
      }
      return result;
    },
  },
  device_serial_number: {
    cluster: 'seMetering',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
      const result = {};
      if (msg.data.hasOwnProperty('meterSerialNumber')) {
        const data = msg.data['meterSerialNumber'];
        result.serial_number = data.toString();
		  }
      return result;
    },
  },
};


const tzLocal = {
  device_preset: {
    key: ['preset'],
    convertSet: async (entity, key, value, meta) => {
      const values = {
        energy: value.energy_preset,
        divisor: value.divisor_preset,
        serial_number: value.serial_number_preset,
        device_name: value.device_name_preset,
      };
      
      // if (values.energy == 0) 
      //   throw new Error(`Invalid energy value: ${values.energy}. The value must be greater than 0.`);
      
      // if (values.serial_number <= 0)
      //   throw new Error(`Invalid serila number value: ${values.serial_number}. The value must be greater than 0.`);
      
      // if (values.divisor % 100)
      //   throw new Error(`Invalid divisor value: ${values.divisor}. The value must be a multiple of 100.`);
        
      // if (values.divisor <= 0)
      //   throw new Error(`Invalid divisor value: ${values.divisor}. The value must be greater than 0.`);
      
      if (values.energy !== undefined && values.energy > 0) {
        const energy_preset = parseFloat(values.energy) * 100;
        await entity.write('seMetering', {0xf000: {value: energy_preset, type: 0x25}})
      }

      const divisor_preset = parseInt(values.divisor, 10);
      if (divisor_preset > 0 && divisor_preset % 100 == 0)
        await entity.write('seMetering', {0xf001: {value: divisor_preset, type: 0x21}})
        
      if (values.serial_number !== undefined && values.serial_number > 0) {
        const serial_num_preset = String(values.serial_number);
        await entity.write('seMetering', {0xf002: {value: serial_num_preset, type: 0x41}})
      }

      if (values.device_name !== undefined) {
        const name_preset = String(values.device_name);
        if (name_preset != "")
          await entity.write('seMetering', {0xf004: {value: name_preset, type: 0x41}})
      }

      return {};
    },
  
  },
  device_number_of_digits: {
    key: ['number_of_digits'],
    convertSet: async (entity, key, rawValue, meta) => {
      const value = switchNumberOfDigits.indexOf(rawValue);
      const payloads = {
        number_of_digits: ['seMetering', {0xf003: {value, type: 0x30}}],
      };
      await entity.write(payloads[key][0], payloads[key][1]);
      return {
        state: {[key]: rawValue},
      };
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('seMetering', [0xf003]);
    },
  },
  device_current_summ_delivered: {
    key: ['current_summ_delivered'],
    convertSet: async (entity, key, value, meta) => {
      return null;
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('seMetering', ['currentSummDelivered']);
    },
  },
  device_name: {
    key: ['device_name'],
    convertSet: async (entity, key, value, meta) => {
      return null;
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('seMetering', [0xf004]);
    },
  },
  device_divisor: {
    key: ['divisor'],
    convertSet: async (entity, key, value, meta) => {
      return null;
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('seMetering', [0xf001]);
    },
  },
  device_serial_number: {
    key: ['serial_number'],
    convertSet: async (entity, key, value, meta) => {
      return null;
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('seMetering', ['meterSerialNumber']);
    },
  },
  device_temperature: {
    key: ['device_temperature'],
    convertSet: async (entity, key, value, meta) => {
      return null;
    },
    convertGet: async (entity, key, meta) => {
      await entity.read('genDeviceTempCfg', ['currentTemperature']);
    },
  },
};


const definition = {
  zigbeeModel: ['ECM_DIN1_counter_B85'],
  model: 'ECM_DIN1_counter_B85',
  vendor: 'Slacky-DIY',
  description: 'Electricity Meter from pulse output',
//  icon: 'data:image/png;base64,',
  fromZigbee: [fzLocal.device_current_summ_delivered,
               fzLocal.device_name,
               fzLocal.device_serial_number,
               fzLocal.device_number_of_digits,
               fzLocal.device_divisor,
               fz.device_temperature], // We will add this later
    toZigbee: [tzLocal.device_number_of_digits,
               tzLocal.device_preset,
               tzLocal.device_current_summ_delivered,
               tzLocal.device_name,
               tzLocal.device_serial_number,
               tzLocal.device_divisor,
               tzLocal.device_temperature],
    meta: {
        multiEndpoint: true
    },
  configure: async (device, coordinatorEndpoint, logger) => {
    const endpoint = device.getEndpoint(1);
    await endpoint.read('genDeviceTempCfg', ['currentTemperature']);
    await endpoint.read('seMetering', ['currentSummDelivered']);
    await endpoint.read('seMetering', [0xF000]);
    await endpoint.read('seMetering', [0xF001]);
    await endpoint.read('seMetering', [0xF002]);
    await endpoint.read('seMetering', [0xF003]);
    await endpoint.read('seMetering', [0xF004]);
    await reporting.bind(endpoint, coordinatorEndpoint, ['seMetering', 'genDeviceTempCfg']);
    await reporting.currentSummDelivered(endpoint, {min: 0, max: 300, change: 0});
    const payload_serial_number = [{attribute: {ID: 0x0308, type: 0x41}, minimumReportInterval: 0, maximumReportInterval: 3600, reportableChange: 0}];
    await endpoint.configureReporting('seMetering', payload_serial_number);
    const payload_temperature = [{attribute: {ID: 0x0000, type: 0x29}, minimumReportInterval: 0, maximumReportInterval: 3600, reportableChange: 0}];
    await endpoint.configureReporting('genDeviceTempCfg', payload_temperature);
    const payload_divisor = [{attribute: {ID: 0xF001, type: 0x21}, minimumReportInterval: 0, maximumReportInterval: 3600, reportableChange: 0}];
    await endpoint.configureReporting('seMetering', payload_divisor);
    const payload_number_of_digits = [{attribute: {ID: 0xF003, type: 0x30}, minimumReportInterval: 0, maximumReportInterval: 3600, reportableChange: 0}];
    await endpoint.configureReporting('seMetering', payload_number_of_digits);
    const payload_device_name = [{attribute: {ID: 0xF004, type: 0x41}, minimumReportInterval: 0, maximumReportInterval: 3600, reportableChange: 0}];
    await endpoint.configureReporting('seMetering', payload_device_name);
  },

  exposes: [
    e
      .numeric('device_temperature', ea.STATE_GET)
      .withUnit('°C')
      .withDescription('Device temperature'),
    e
      .numeric('current_summ_delivered', ea.STATE_GET)
      .withUnit('kWh')
      .withDescription('Tariff Summation'),
    e
      .text('device_name', ea.STATE_GET)
      .withDescription('Device Name'),
    e
      .text('serial_number', ea.STATE_GET)
      .withDescription('Device Serial Number'),
    e
      .numeric('divisor', ea.STATE_GET)
      .withDescription('Scale 1 kWh in pulses (multiple of 100)'),
    e
      .composite('preset', 'preset', ea.STATE_SET)
      .withFeature(e.text('device_name_preset', ea.STATE_SET)
                  .withDescription('Device Name Preset'))
      .withFeature(e.numeric('serial_number_preset', ea.STATE_SET)
                  .withDescription('Device Serial Number Preset'))
      .withFeature(e.numeric('energy_preset', ea.STATE_SET)
                  .withDescription('Energy Preset (example: 12345,45)')
                  .withValueStep(0.01))
      .withFeature(e.numeric('divisor_preset', ea.STATE_SET)
                  .withDescription('Scale 1 kWh in pulses (multiple of 100)')
                  .withValueStep(100)),
    e
      .enum('number_of_digits', ea.ALL, ['5 Digits', '6 Digits'])
      .withDescription('Number of Digits to the left of the Decimal Point'),
  ],
              
  ota: true,

};

module.exports = definition;

