import NavButtons from '../../components/NavBar/NavButtons';
import './Home.css'

/** Landing Page: where the app starts */
export default function Home() {
  return (
    <>
      <header>
        {/* Logo, header, subheader */}
        <img className="menu_logo" src="/src/assets/grad-cap-logo.svg" alt="graduation cap" />
        <h1 className="homepage-header">Campus Tour Program</h1>
        <p className="homepage-subheader">Tour campuses, view colleges, purchase souvenirs!</p>
      </header>
      <main>
        {/* Links to pages: component NavButtons holds actual routes to components */}
        <div className="menu_selection">
          <NavButtons />
        </div>
      </main>
    </>
  )
}
