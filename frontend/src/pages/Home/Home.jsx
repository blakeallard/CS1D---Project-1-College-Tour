import NavButtons from '../../components/NavBar/NavButtons';
import AOS from 'aos';
import 'aos/dist/aos.css';
import './Home.css'
import { useEffect } from 'react';

/** Landing Page: where the app starts */
export default function Home() {
  useEffect(() => {
    AOS.init();
  }, []);

  return (
    <>
      <header data-aos="zoom-in" data-aos-duration="600">
        {/* Logo, header, subheader */}
        <img className="menu_logo" src="/src/assets/grad-cap-logo.svg" alt="graduation cap" />
        <h1 className="homepage-header">Campus Tour Program</h1>
        <p className="homepage-subheader">Tour campuses, view colleges, purchase souvenirs!</p>
      </header>
      <main data-aos="zoom-in" data-aos-duration="600">
        {/* Links to pages: component NavButtons holds actual routes to components */}
        <div className="menu_selection">
          <NavButtons />
        </div>
      </main>
    </>
  )
}
