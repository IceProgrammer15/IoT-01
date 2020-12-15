import {httpGet, httpPost} from '.'; 

const Urls = {
    setPin : 'setpin',
    scan: 'scan',
    connect: 'connect',
}


export const setPinValue = async (pin, state) =>{
    const response = await httpPost(Urls.setPin,{pin,state});
    return response;
}

export const scanNetwork = async ()=>{
    const response = await httpGet(Urls.scan);
    return response;
}


export const connectToNetwork = async (ssid,password)=>{
    const response = await httpPost(Urls.connect,{ssid,password});
    return response;
}