## Worksheet 2 

### Overview

This project is an IoT eBike Fleet Monitoring system designed to track eBike locations and monitor their maintenance status. The system consists of:
- A **Gateway** that receives UDP messages from eBike clients, converts incoming JSON data to GeoJSON features, and serves a web interface displaying a map and a data table.
- An **eBike Client** that simulates sending GPS (and other sensor) data via UDP to the gateway.
- A **Management Client** that sends commands (e.g., lock/unlock) to the gateway for maintenance management.

### Activity One: Displaying eBike Locations

**Objective:**  
Enable the system to display eBike locations on a web map.

**Process:**
- Reused and adapted code from Worksheet 1 to allow the eBike client to send GPS data in JSON format.
- Modified the **GPSSensor** class to generate properly formatted JSON strings containing eBike ID, timestamp, latitude, and longitude.
- Developed a **MessageHandler** in the gateway to parse these messages and convert them into GeoJSON features.
- Created a web server using Poco's HTTPServer to serve a map (and a data table) showing the received eBike locations.
- Configured the Makefile to compile and link all components correctly.

**Challenges:**
- Initially, only one eBike was displayed, and status information was missing from the display.
- Port mismatches caused the web server to bind to the wrong port (e.g., port 0).
- Linking issues with the simulated socket class required marking its member functions as inline.
- The Makefile needed adjustments to correctly set object file paths.

<img src="(<map iot.png>)" alt="structure" width="200"/>

### Activity Two: Maintenance Management of eBikes

**Objective:**  
Enhance the system to support maintenance management by enabling locking/unlocking of eBikes and displaying status information.

**Process:**
- Extended the JSON data format to include a `lock_status` (or `status`) field to indicate whether an eBike is locked or unlocked.
- Updated the **MessageHandler** to incorporate this status into the GeoJSON features that the gateway serves.
- Developed a separate **Management Client** that sends COMMAND messages (e.g., lock/unlock commands) to the gateway.
- Updated the front-end (in map.html and its JavaScript) to display additional columns such as eBike ID, status, coordinates, and timestamp.
- Simulated multiple eBikes by running multiple instances of the eBike client with different eBike IDs and CSV files.

**Challenges:**
- The management client did not exist initially; I had to create a new source file for it.
- Duplicate declarations in `ebikeGateway.cpp` were resolved to avoid conflicts.
- Ensuring that the gateway correctly appended new eBike features instead of overwriting existing ones.
- Front-end JavaScript required modification to loop through all features and correctly display the additional data.

### Final Reflections

This project significantly improved my understanding of network programming, especially in handling UDP communication and JSON parsing using the Poco libraries. I learned the importance of:
- **Attention to detail:** Properly managing ports, JSON structure, and build configurations is critical.
- **Incremental development:** Building the basic functionality first allowed for focused troubleshooting before integrating advanced features.
- **Troubleshooting:** Resolving linking errors, environment configuration, and front-end integration issues enhanced my problem-solving skills.
- **Documentation:** Clearly documenting the process and challenges provided valuable insights and will help in future projects.

Overall, while I encountered several challenges during development, each issue provided a learning opportunity, and the final system successfully demonstrates both location tracking and basic maintenance management features.


