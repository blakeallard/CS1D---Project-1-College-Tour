import { NavLink } from "react-router";
import HomeButton from "../../components/HomeButton/HomeButton";
import "./Tours.css";

/** Tours page: shows user options for different kinds of tours */
export default function Tours() {
    return (
        <>
            {/* Link to homepage */}
            <HomeButton />
            <header data-aos="zoom-in" data-aos-duration="600">
                <h1 className="tours-header">Select a Tour</h1>
            </header>
            <main data-aos="zoom-in" data-aos-duration="600">
                {/* Navigation for selecting tour type, each option navigates to a different page */}
                <nav className="tours-nav_buttons-container">
                    <NavLink className="nav-button" to={"/saddleback-start"}>Saddleback</NavLink>
                    <NavLink className="nav-button" to={"/asu-start"}>ASU</NavLink>
                    <NavLink className="nav-button" to={"/uci-start"}>UCI</NavLink>
                    <NavLink className="nav-button" to={"/custom-tour"}>Create a Custom Tour</NavLink>
                </nav>
            </main>
        </>
    );
}