import { useState } from 'react'
import './App.css'

function App() {
  const [selected, setSelected] = new useState("none");
  const [submit, setSubmit] = new useState(false);

  const handleSelected = (e) => {
    setSelected(e.target.value);
    console.log("selected value: ", e.target.value);
  }

  const checkSelected = () => {
    console.log("Clicked!");
    if(selected != "none") {
      setSubmit(true);
      setTimeout(() => {
        setSubmit(false);
      }, 3000);
    }
  }

  const setSubmitStatusValue = () => {
    switch(selected) {
      case "saddleback_start" :
        return "Saddleback";
      case "asu_start" :
        return "ASU"
      case "custom_start" :
        return "Custom"
      default :
        break;
    }
  }

  return (
    <>
      <div className="selection_status">{submit ? setSubmitStatusValue() + " Tour Selected!" : ""}</div>
      <h1>Campus Tours</h1>
      <img className="menu_logo" src="/src/assets/grad-cap-logo.svg" alt="graduation cap" />
      <div className="menu_selection">
        <p>Choose your tour:</p>
        <select name="tour_type_selection" onChange={handleSelected}>
          <option value="none">---</option>
          <option value="saddleback_start">Saddleback</option>
          <option value="asu_start">ASU</option>
          <option value="custom_start">Custom</option>
        </select>
        <div className="button_container">
          <button type="button" onClick={checkSelected}>Submit</button>
        </div>
      </div>
    </>
  )
}

export default App
