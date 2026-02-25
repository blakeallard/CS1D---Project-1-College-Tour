import { NavLink } from "react-router";
import { useState } from "react";
import AdminLogin from "../AdminLogin/AdminLogin";
import "./NavButtons.css"

/** Navigation for landing page.
 * 
 * Takes user to one of three sub-pages: Tour, Campuses, Admin
 */
export default function NavButtons() {
    // STATE VARIABLES:
    // Holds whether admin login was clicked or not (boolean)
    const [adminClicked, setAdminClicked] = new useState(false);

    /** Toggle adminClicked to opposite boolean value */
    const handleAdminClicked = () => {
        setAdminClicked(!adminClicked);
    }

    return (
        <nav className="nav_buttons-container">
            <NavLink className="nav-button" to={"/tour"}>Start a Tour</NavLink>
            <NavLink className="nav-button" to={"/campuses"}>View Campuses</NavLink>
            {/* When button clicked, toggles adminClicked boolean value */}
            <button className="nav-button" onClick={handleAdminClicked}>Administrator Login</button>
            {/* If adminClicked is "true" then show AdminLogin component, else show nothing */}
            {adminClicked ? <AdminLogin handleAdminClicked={handleAdminClicked} /> : ""}
        </nav>
    );
}