# RF Gateway
Targets a Raspberry pi server to act as a gateway between the [https://github.com/HomeSmartMesh/IoT_Frameworks/tree/master/stm32_rf_dongle/rf_bridge](STM32 RF Gateway dongle) and Mosquitto as MQTT Broquer

# Usage
### `scons` 
to build

### `./gateway` 
to run, the config file used is `"/mesh_config/rf_gateway_config.json"`

#C, Cpp clients
##Install
```bash
apt-get install libmosquitto-dev
sudo apt-get install libmosquittopp-dev
```
