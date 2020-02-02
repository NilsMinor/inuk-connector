# inuk-connector

The inuk-connector application aims to act as a mqtt bridge between the inuk bluetooth-mesh (based on FruityMesh) and the intern.
A serial interface is used to connect to a NRF51/NRF52 module, which receives messages from the mesh.The messages are parsed and
converted to json object to send the via an mqtt-client.

This application is based on qmqtt.

## TODO
- add config file to load serial connection settings
- add config file to load mqtt connection settings

## topics

After a connection is established to an mqtt server, the connector will create a gateway topic,

### structure

GW_ID ist the ID of the Gateway for example 1234

inuit/$gatewayID
                /status
                /nodes
                       /unenrolled/$nodeID
                       /enrolled/$nodeID





