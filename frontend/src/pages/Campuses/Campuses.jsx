import { useState, useEffect } from "react";
import axios from 'axios';
import HomeButton from "../../components/HomeButton/HomeButton";
import "./Campuses.css";

export default function Campuses() {
    const [campuses, setCampuses] = new useState([]);

    const handleCampuses = (campusData) => {
        setCampuses(campusData);
    }

    useEffect(() => {
        axios.get("/api/University/all")
            .then(response => {
                const allCampuses = response.data.campuses;
                // console.log(allCampuses);
                handleCampuses(allCampuses);
            });
    }, []);

    return (
        <>
            <HomeButton />
            <h1>Campus Information</h1>
            <div className="campus_info-container">
                <table>
                    <thead>
                        <tr>
                            <th>Name</th>
                            <th>Distance from Saddleback</th>
                        </tr>
                    </thead>
                    <tbody>
                        {campuses.map(campus => (
                            <tr>
                                <td>{campus.name}</td>
                                <td>{campus.distance}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>
        </>
    );
}