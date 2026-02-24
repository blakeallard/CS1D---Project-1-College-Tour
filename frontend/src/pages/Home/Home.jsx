// import { useState } from 'react'
import NavButtons from '../../components/NavBar/NavButtons';
import './Home.css'

export default function Home() {
  // const [selected, setSelected] = new useState("none");

  return (
    <>
      <header>
        <h1 className="homepage-header">Campus Tours</h1>
        <img className="menu_logo" src="/src/assets/grad-cap-logo.svg" alt="graduation cap" />
      </header>
      <main>
        <div className="menu_selection">
          <NavButtons />
        </div>
      </main>
    </>
  )
}
