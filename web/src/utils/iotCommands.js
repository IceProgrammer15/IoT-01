import {httpPost} from '.'; 

const setPinUrl = 'setpin';

export const setDigitalPin = async (pin, state) =>{
    const response = await httpPost(setPinUrl,{pin,state});
    return response;
}