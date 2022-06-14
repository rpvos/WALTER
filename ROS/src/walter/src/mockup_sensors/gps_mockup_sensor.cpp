#include "gps_mockup_sensor.hpp"
#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>

namespace gps_mockup_sensor {

Sensor::Sensor(double frequency, double longitude, double latitude,
               double altitude) {
  this->node_handle = new ros::NodeHandle;
  this->loop_rate = new ros::Rate(frequency);
  this->publisher =
      node_handle->advertise<sensor_msgs::NavSatFix>("/gps/fix", 1000);
  this->sequence = 1;
  this->longitude = longitude;
  this->latitude = latitude;
  this->altitude = altitude;
}
Sensor::~Sensor() { ; }

void Sensor::spin() {
  // Create header for message
  std_msgs::Header header;
  header.frame_id = "gps";

  // Create nav sat status message
  sensor_msgs::NavSatStatus nav_sat_status;
  nav_sat_status.status = sensor_msgs::NavSatStatus::STATUS_FIX;
  nav_sat_status.service = sensor_msgs::NavSatStatus::SERVICE_GPS;

  // Create nav sat fix message
  sensor_msgs::NavSatFix nav_sat_fix;
  nav_sat_fix.position_covariance_type =
      sensor_msgs::NavSatFix::COVARIANCE_TYPE_UNKNOWN;
  nav_sat_fix.latitude = this->latitude;
  nav_sat_fix.longitude = this->longitude;
  nav_sat_fix.altitude = this->altitude;
  nav_sat_fix.status = nav_sat_status;

  while (ros::ok()) {
    // Set sequence in header and increment sequence
    header.seq = sequence++;
    // Set the time this message is being constructed
    header.stamp = ros::Time::now();
    // Set header of nav_sat_fix
    nav_sat_fix.header = header;

    publisher.publish(nav_sat_fix);

    // Sleep for the remaining time
    loop_rate->sleep();
  };
}

} // namespace gps_mockup_sensor
