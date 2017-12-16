/*
The MIT License (MIT)

Copyright (c) 2017 Wassim Filali

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

___________________________________________________________________________________
 dependencies :
 - Poco,Boost filesystem : using The Boost Software License 1.0
 - bme_280 : with datasheet's associated rights see bme280_server.hpp
 - json.hpp MIT License Copyright (c) 2013-2017 Niels Lohmann <http://nlohmann.me>.

___________________________________________________________________________________

 start date : 20.01.2017
 takeopver into rf_gateway : 13.12.2017

 
*/


//for usleep() sleep()
#include <unistd.h>
//for printf
#include <stdio.h>
//for stdout
#include <iostream>
//for file
#include <fstream>
//for abs
#include <cmath>

//for exit()
#include <cstdlib>


#include "serial.hpp"

#include "utils.hpp"

#include "mqtt_rf.hpp"
#include "mesh.hpp"

#include <assert.h>
#include <string>
#include <memory>

#include "json.hpp"
using json = nlohmann::json;

#include "log.hpp"

#include <boost/filesystem.hpp>
//using namespace boost::filesystem;

using namespace std;

json read_json(std::string const &filename)
{
	if (boost::filesystem::exists(filename))
	{
		std::cout << "________________________________________________________________"<< std::endl;
		std::cout << "Loading : "<< filename << std::endl;
	}
	else
	{
		std::cout << "File not found : "<< filename << std::endl;
		exit(1);
	}
	std::ifstream data_file(filename);
	json result;
	data_file >> result;
	return result;
}

int main(int argc, const char *argv[]) 
{
	boost::filesystem::path app_path(argv[0]);

	json config = read_json(app_path.parent_path().string()+"/rf_gateway_config.json");

	json calib = read_json(app_path.parent_path().string()+"/mesh_config/bme280_calibration.json");

	Log::config(config["log"]);
	
	Serial 			stream(config["serial"],calib);	// - process serial port stream : - calibrate sensors values
									// - provides ready to store measures MAP of Nodes.Sensors.Values,Timestamp
									// - If not configured to be used then the .update() polling is neutral

	mqtt_rf_c			mqtt(config["mqtt_client"],stream);	//MQTT client app wrapper, will attempt connection on creation if params provided

	//could be retrived from parsing the dongle startup or by querry
	mesh::set_source_nodId(config["mqtt_client"]["dongle_node_id"]);

	//#2 issue, it is likely that someone else is using the port in parallel
	//discard first trash buffer if available right after opening the port
	//this discard measure is not enough as ibberish appears still
	stream.update();
	
	std::cout << "______________________Main Gateway Loop______________________" << std::endl;
	while (1) 
	{
		if(stream.update())
		{
			NodeMap_t measures = stream.processBuffer();
			if(measures.size() != 0)
			{
				mqtt.publish_measures(measures);
			}
		}

		//run() contains the loop needed to process certain QoS messages and reconnect if connection lost
		mqtt.run();
		
		//5 ms : this is an unnneccessary load if the processing grows up
		usleep(5000);
		
	}

	return 0;
}
