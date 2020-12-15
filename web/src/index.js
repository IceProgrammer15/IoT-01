import "./style.scss";
import { Component } from "preact";
import { useState } from "preact/hooks";

import {setDigitalPin} from './utils/iotCommands';
import Switch from "./components/switch";

export default (App) => {


  // GPIO-0, GPOI-1(led), GPIO-2
  const [digitalState, setPinState] = useState([0,1,0]);


  const setPin = (pinNumber, pinState)=>{
    console.log({pinNumber, pinState});
      const newState = [...digitalState];
      newState[pinNumber] = pinState;
      setPinState(newState);
  }


  const switches = digitalState.map((pinState,index)=>
    (
      <div class="f-row">
      <b class="mr1">{`GPIO-${index}`}</b>
      <Switch
        id={`sw${index}`}
        checked={pinState==1}
        onChange={(checked) => {
          setDigitalPin(index, checked?1:0).then(res=>{
            console.log(res);
            if(res && !res.error && res.data.state != void 0){              
              setPin(index,checked?1:0);
            }
          });          
        }}
      />
    </div>
    )
  );

  return (
    <div>
      
		<h2 style={{textAlign:'center'}}>ESP8266</h2>
      {switches}      
    </div>
  );
};
