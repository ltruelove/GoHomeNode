#ifndef PAGE_TEXT
#define PAGE_TEXT

  const char wifi_index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
  <html><h3>GoHome Node WiFi Selection</h3>
  <p><a href='/wifi-setup'>Go To WiFi Form</a></p>
  <p><a href='/clear'>Clear Preferences (CAREFUL!)</a></p>
  </html>)rawliteral";

    const char wifi_select_form[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
    <!DOCTYPE HTML>
    <html>
        <h3>GoHome Node WiFi Setup</h3>
        <p>%IPPLACEHOLDER%</p>
        <form method='get' action='setting'>
            <table>
                <tr>
                    <td width='100'><label>SSID: </label></td>
                    <td>%WIFIOPTIONS%</td>
                </tr>
                <tr>
                    <td><label>Key: </label></td>
                    <td><input type='password' name='pass' value='' length=64></td>
                </tr>
                <tr>
                    <td><label>Node Name: </label></td>
                    <td><input type='text' name='name'></td>
                </tr>
                <tr>
                    <td><label>API Host: </label></td>
                    <td><p>http:// <input type='text' name='apiHost' value='' length=32></p></td>
                </tr>
                <tr>
                    <td><label>API PORT: </label></td>
                    <td><input type='number' name='apiPort' value='' length=5></td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                    <td><input type='submit'></td>
                </tr>
            </table>
        </form>
    </html>
    )rawliteral";

    const char register_index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
    <html>
        <h3>GoHome Node Registration</h3>
        <p><a href='/register'>Go To Registration Form</a></p>
        <p><a href='/clear'>Clear Preferences (CAREFUL!)</a></p>
    </html>)rawliteral";

    const char register_node_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
    <html>
        <head>
        <script type='text/javascript'>
            var apiHost = '%APIHOST%';
        </script>
            <script src='https://code.jquery.com/jquery-3.6.0.min.js' integrity='sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=' crossorigin='anonymous'></script>
            <script type='text/javascript' src='%APIHOST%/js/registerNode.js'></script>
        </head>
        <body>
            <h3>GoHome Node Registration</h3>
            <form method='get' action='setting'>
                <input type='hidden' id='mac' value='%NODEMAC%'>
                <input type='hidden' id='nodeName' value='%NODENAME%'>
                <table>
                    <tr>
                        <td><label>Name: </label></td>
                        <td><input type='text' id='typeName' value=''></td>
                    </tr>
                    <tr>
                        <td width='100'><label>Control Point: </label></td>
                        <td><select id='nodeControlPoint'></select></td>
                    </tr>
                    <tr>
                        <td><label>Type: </label></td>
                    </tr>
                    <tr>
                        <td><label><input type='radio' name='sType' value='sensor' checked='checked' > Sensor</label></td>
                        <td><label><input type='radio' name='sType' value='switch'> Switch</label></td>
                    </tr>
                    <tr id='sensorTypes'>
                        <td><label>Sensor Types: </label></td>
                        <td><select id='sensorTypeOptions'></select></td>
                    </tr>
                    <tr class='dhtTypes'>
                        <td><label>Select DHT Type: </label></td>
                    </tr>
                    <tr class='dhtTypes'>
                        <td><label><input type='radio' name='dhtType' value='11' checked='checked' > DHT11</label></td>
                        <td><label><input type='radio' name='dhtType' value='22'> DHT22</label></td>
                    </tr>
                    <tr id='switchTypes'>
                        <td><label>Switch Types: </label></td>
                        <td><select id='switchTypeOptions'></select></td>
                    </tr>
                    <tr class='switchOptions'>
                        <td><label>Switch Options: </label></td>
                    </tr>
                    <tr class='switchOptions'>
                        <td><label>Momentary press duration (ms): </label></td>
                        <td><input type='number' id='MomentaryPressDuration' value='0'></td>
                    </tr>
                    <tr class='switchOptions'>
                        <td><label>Closed toggle status: </label></td>
                    </tr>
                    <tr class='switchOptions'>
                        <td><label><input type='radio' name='IsClosedOn' value='1'> On</label></td>
                        <td><label><input type='radio' name='IsClosedOn' value='0'> Off</label></td>
                    </tr>
                    <tr>
                        <td><label>Pin: </label></td>
                        <td><input type='number' name='pin' id='pin' value=''></td>
                    </tr>
                    <tr>
                        <td></td>
                        <td><button id='addTypeButton'>Add</button></td>
                    </tr>
                    <tr>
                        <td><label>Sensors: </label></td>
                        <td><div id='sensorList'></div></td>
                    </tr>
                    <tr>
                        <td><label>Switches: </label></td>
                        <td><div id='switchList'></div></td>
                    </tr>
                    <tr>
                        <td>&nbsp;</td>
                        <td><input type='submit' value='Save'></td>
                    </tr>
                </table>
            </form>
        </body>
    </html>
    )rawliteral";
#endif