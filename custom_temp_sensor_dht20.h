// /config/esphome/custom_temp_sensor.h

#include "esphome.h"
#include "Wire.h"
#include "DHT20.h"

class CustomTempSensorDH20 : public PollingComponent, public Sensor {
public:
	DHT20 DHT;

	Sensor *temperature_sensor = new Sensor();
	Sensor *humidity_sensor = new Sensor();

	double temp = 0;
	double humidity = 0;

	float get_setup_priority() const override { return esphome::setup_priority::DATA; }

	CustomTempSensorDH20() : PollingComponent(5000) { }

	void setup() override {
		Wire.setClock(400000);
		Wire.begin();
		DHT.begin();
	}

	void update() override {
		uint32_t start = micros();
		int status = DHT.read();
		uint32_t stop = micros();
		switch (status) {
			case DHT20_OK:
				ESP_LOGD("custom", "OK,\t");
			break;
			case DHT20_ERROR_CHECKSUM:
				ESP_LOGD("custom", "Checksum error,\t");
			break;
			case DHT20_ERROR_CONNECT:
				ESP_LOGD("custom", "Connect error,\t");
			break;
			case DHT20_MISSING_BYTES:
				ESP_LOGD("custom", "Missing bytes,\t");
			break;
			default:
				ESP_LOGD("custom", "Unknown error (status) ,\t");
			break;
		}

		temp = DHT.getTemperature();
		temperature_sensor->publish_state(DHT.getTemperature());

		humidity = DHT.getHumidity();
		humidity_sensor->publish_state(DHT.getHumidity());
	}
};
