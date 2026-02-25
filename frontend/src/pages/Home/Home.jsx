// import { useState } from 'react'
import NavButtons from '../../components/NavBar/NavButtons';
import './Home.css'

export default function Home() {
  // const [selected, setSelected] = new useState("none");

  return (
    <>
      <header>
        <img className="menu_logo" src="/src/assets/grad-cap-logo.svg" alt="graduation cap" />
        <h1 className="homepage-header">Campus Tour Program</h1>
        <p className="homepage-subheader">Tour campuses, view colleges, purchase souvenirs!</p>
      </header>
      <main>
        <div className="menu_selection">
          <NavButtons />
        </div>
      </main>
    </>
  )
}
