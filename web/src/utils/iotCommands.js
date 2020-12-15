import {httpPost} from '.'; 

const setPinUrl = 'setpin';

export const setPinValue = async (pin, state) =>{
    const response = await httpPost(setPinUrl,{pin,state});
    return response;
}