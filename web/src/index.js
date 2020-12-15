import "./style.scss";
import { Component } from "preact";
import { useState } from "preact/hooks";

import {setPinValue} from './utils/iotCommands';
import Slider from "./components/slider";


export default (App) => {


  // GPIO-0, GPOI-1(led), GPIO-2
  const [analogState, setPinState] = useState([0,1024,0]);



  const setPin = (pinNumber, pinState)=>{
      const newState = [...analogState];
      newState[pinNumber] = pinState;
      setPinState(newState);
  }


  const sliders = analogState.map((pinState,index)=>
    (
      <div class="f-row">
        <b class="mr1">{`GPIO-${index}`}</b>
        <Slider value={pinState} onChange={(value)=>          
          setPinValue(index, value).then(res=>{
            if(res && !res.error && res.data.state != void 0){              
              setPin(index,value)
            }
          })} />
      </div>
    )
  );

  return (
    <div>
      
		<h2 style={{textAlign:'center'}}>ESP8266</h2>
      {sliders}   
    </div>
  );
};
