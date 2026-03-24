import HomeButton from "@components/HomeButton/HomeButton";
import { NavLink } from "react-router";
import styles from "./Admin.module.css";

export default function Admin() {
    return (
        <div className={styles.adminPage}>
            <HomeButton />

            <h1 data-aos="zoom-in" data-aos-duration="600">
                Administrator Operations
            </h1>

            <nav className={styles.navButtonsContainer}>
                <NavLink className={styles.navButton} to="/admin/import-data">
                    Import Colleges/Souvenirs
                </NavLink>

                <NavLink className={styles.navButton} to="/admin/add-svnrs">
                    Add Souvenirs
                </NavLink>

                <NavLink className={styles.navButton} to="/admin/delete-svnrs">
                    Delete Souvenirs
                </NavLink>

                <NavLink className={styles.navButton} to="/admin/edit-svnrs">
                    Edit Souvenirs
                </NavLink>
            </nav>
        </div>
    );
}
