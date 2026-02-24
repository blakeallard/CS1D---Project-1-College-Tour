import { NavLink } from "react-router";

export default function NavButtons() {
    return (
        <nav className="nav_buttons-container">
            <NavLink className="nav-button" to={"/tour"}>Start a Tour</NavLink>
            <NavLink className="nav-button" to={"/campuses"}>View Campuses</NavLink>
            <NavLink className="nav-button" to={"/admin"}>Administrator Login</NavLink>
        </nav>
    );
}