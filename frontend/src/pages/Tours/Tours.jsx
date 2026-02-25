import { NavLink } from "react-router";
import HomeButton from "../../components/HomeButton/HomeButton";
import "./Tours.css";

export default function Tours() {
    return (
        <>
            <HomeButton />
            <header>
                <h1 className="tours-header">Select a Tour</h1>
            </header>
            <main>
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