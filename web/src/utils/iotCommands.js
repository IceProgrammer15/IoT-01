import {httpPost} from '.'; 

const setPinUrl = 'http://192.168.4.1/setpin';

export const setDigitalPin = async (pin, state) =>{
    const response = await httpPost(setPinUrl,{pin,state});
    console.log({response});
}