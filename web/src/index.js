import "./style.scss";
import { Component } from "preact";
import { useState } from "preact/hooks";

import {setDigitalPin} from './utils/iotCommands';
import Switch from "./components/switch";

export default (App) => {
  const [pin1, setPin1] = useState(false);
  const [pin2, setPin2] = useState(false);
  const [pinNumber, setPinNumber] = useState(0);

  return (
    <div>
      <input type='number' value={pinNumber} onChange={(e)=>{setPinNumber(+e.target.value)}}/>
		<h2 style={{textAlign:'center'}}>ESP8266</h2>
      <div class="f-row">
        <b class="mr1">GPIO-0</b>
        <Switch
          id="sw1"
          checked={pin1}
          onChange={(checked) => {
            setDigitalPin(pinNumber, checked?1:0).then(res=>{
              if(!res.error && res.data.state){
                setPin1(+res.data.state);
              }
            });
            
          }}
        />
      </div>

      <div class="f-row">
        <b class="mr1">GPIO-2</b>

        <Switch
          id="sw2"
          checked={pin2}
          onChange={(checked) => setPin2(checked)}
        />
      </div>
    </div>
  );
};
