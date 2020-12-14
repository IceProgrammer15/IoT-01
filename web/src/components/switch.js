
const Switch = (props) => {
    
  return (
    <span class={props.color || "green"}>
      <input id={props.id} class="tgl tgl-ios"  type="checkbox" 
        checked={props.checked}
        onChange= {(e)=>props.onChange && props.onChange(e.target.checked)}
      />
      <label class="tgl-btn " for={props.id}></label>
    </span>
  );
};

export default Switch;
