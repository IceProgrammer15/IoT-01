

const Slider = (props)=>{


    return(
    
        <input class="slider" type="range" min="0" max="1023" value={props.value}
        onChange={(e)=>props.onChange && props.onChange(e.target.value)}
        />
       
    )
}

export default Slider;