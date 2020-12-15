import "./style.scss";
import { Component } from "preact";
import { useState } from "preact/hooks";
import {setPinValue, scanNetwork, connectToNetwork} from './utils/iotCommands';


export default (App) => {

  const [wifiList, setWifiList] = useState([]);
  const [selectedIndex, setIndex] = useState(-1);
  const [status, setStatus] = useState(null);


  const selectList = wifiList.map((item,index)=>(
    <div class={["list-item",selectedIndex==index?"selected":""].join(' ')} onClick={()=>{
        setIndex(index == selectedIndex ? -1 : index);
    }}>
      {item}
    </div>
  ));

  return (
    <div class="container">
      
      <div style={{textAlign:'center'}}>
      <h2 >ESP8266</h2>
      {status && <small>{status}</small>}
      </div>
		
    
    
        {selectList}
        <div class="f-col">
          <button disabled={!!status} onClick={()=>{
            setStatus('Scanning network...');
            scanNetwork().then(res=>{
              setStatus(null);
              if(res.error){
                console.error(res.error);
                alert('Error scanning for wifi networks!');
                return;
              }
              setWifiList(res.data||[]);
            });
          }}>Scan...</button>

          <button disabled={!!status || selectedIndex<0} onClick={()=>{
            const ssid = wifiList[selectedIndex];
            const password = prompt('Please enter Wifi password');
            setStatus(`Connecting to "${ssid}"`);
            connectToNetwork(ssid,password).then(res=>{
            setStatus(null);
            if(res.error){
              console.error(res.error);
              alert('Error connecting to selected network!');
              return;
            }
              alert(`Connected successfully!\nIP: ${res.data.ip}`);
            });
          }}>Connect...</button>

        </div>
    </div>
  );
};
