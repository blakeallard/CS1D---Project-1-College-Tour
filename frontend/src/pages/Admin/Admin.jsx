import { NavLink } from "react-router";
import HomeButton from "../../components/HomeButton/HomeButton";
import "./Admin.css";

/** Admin Page: where user can perform Admin operations */
export default function Admin() {
    return (
        <>
            <HomeButton />
            <h1 data-aos="zoom-in" data-aos-duration="600">Administrator Operations</h1>
            <nav className="nav_buttons-container" data-aos="zoom-in" data-aos-duration="600">
                <NavLink className="nav-button" to={"/admin/import-data"}>Import Colleges/Sourvenirs</NavLink>
                <NavLink className="nav-button" to={"/admin/add-svnrs"}>Add Souvenirs</NavLink>
                <NavLink className="nav-button" to={"/admin/delete-svnrs"}>Delete Souvenirs</NavLink>
                <NavLink className="nav-button" to={"/admin/edit-svnrs"}>Edit Souvenirs</NavLink>
            </nav>
        </>
    );
}