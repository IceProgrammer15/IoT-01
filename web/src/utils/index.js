export const sendRequest = async (url, method, body) => {
    try {
        const response = await fetch(url, {
            "credentials": "include",
            "headers": {
                "Accept": "*/*",
                "Content-Type": "application/x-www-form-urlencoded",
                "Accept-Language": "en-US,en;q=0.5",
                "Cache-Control": "max-age=0",
                "Accept-Encoding": "gzip, deflate"
            },
            "method": method || "GET",
            "body": body
        });

        const json = await response.json();
        return {
            data: json,
            status: response.status
        }
    } catch (error) {
        return {
            error
        }
    }
}

export const httpGet = async (url, parameters) => {
    const query = parameters || {};
    const queryString = Object.keys(query).reduce((result, key, index) => {
        const value = encodeURIComponent(parameters[key])
        const separator = index == 0 && url.indexOf('?') < 0 ? '?' : '&';
        result = `${result}${separator}${key}=${value}`;
        return result;
    }, "");

    const urlWithQueryString = `${url}${queryString}`;
    return await sendRequest(urlWithQueryString, 'GET');
}


export const httpPost = async (url, parameters) => {

    const query = parameters || {};
    const body = Object.keys(parameters).reduce((result, key, index) => {
        const value = encodeURIComponent(parameters[key])
        result = `${result}${key}=${value}&`;
        return result;
    }, "");
    return await sendRequest(url, 'POST', body);
}