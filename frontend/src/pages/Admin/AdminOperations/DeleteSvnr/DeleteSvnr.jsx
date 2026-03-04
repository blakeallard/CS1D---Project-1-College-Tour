import { useEffect, useState } from "react";
import axios from "axios";
import HomeButton from "../../../../components/HomeButton/HomeButton";
import "./DeleteSvnr.css";

export default function DeleteSvnr() {
    const [schools, setSchools] = useState([]);
    const [selectedSchool, setSelectedSchool] = useState("");

    const [souvenirs, setSouvenirs] = useState([]);
    const [selectedItem, setSelectedItem] = useState("");

    // Load schools on mount
    useEffect(() => {
        axios.get("/api/University/all")
            .then(res => {
                setSchools(res.data.campuses.map(c => c.name));
            })
            .catch(err => console.error(err));
    }, []);

    // Load souvenirs when school changes
    useEffect(() => {
        if (!selectedSchool) return;

        axios.get(`/api/Souvenir/${selectedSchool}`)
            .then(res => {
                setSouvenirs(res.data); // expect [{name, price}]
            })
            .catch(err => console.error(err));
    }, [selectedSchool]);

    const handleDelete = async (e) => {
        e.preventDefault();

        if (!selectedSchool || !selectedItem) {
            alert("Please select a school and item.");
            return;
        }

        try {
            await axios.delete(
                `/api/Souvenir/${selectedSchool}`,
                {
                    data: { item: selectedItem }  // DELETE body must go inside "data"
                }
            );

            alert("Souvenir deleted!");

            // Refresh souvenir list
            const res = await axios.get(`/api/Souvenir/${selectedSchool}`);
            setSouvenirs(res.data);

            setSelectedItem("");

        } catch (err) {
            console.error(err);
        }
    };

    return (
        <>
            <HomeButton />

            <header data-aos="zoom-in" data-aos-duration="600">
                <h1>Delete Souvenir</h1>
            </header>

            <main data-aos="zoom-in" data-aos-duration="600">
                <form onSubmit={handleDelete} className="delete-svnr-form">

                    {/* Select School */}
                    <label>
                        Select School:
                        <select
                            value={selectedSchool}
                            onChange={(e) => {
                                setSelectedSchool(e.target.value);
                                setSelectedItem("");
                            }}
                        >
                            <option value="">-- Choose School --</option>
                            {schools.map(name => (
                                <option key={name} value={name}>
                                    {name}
                                </option>
                            ))}
                        </select>
                    </label>

                    {/* Select Souvenir */}
                    {selectedSchool && (
                        <label>
                            Select Souvenir:
                            <select
                                value={selectedItem}
                                onChange={(e) => setSelectedItem(e.target.value)}
                            >
                                <option value="">-- Choose Item --</option>
                                {souvenirs.map(item => (
                                    <option key={item.name} value={item.name}>
                                        {item.name}
                                    </option>
                                ))}
                            </select>
                        </label>
                    )}

                    {selectedItem && (
                        <button type="submit">
                            Delete Souvenir
                        </button>
                    )}

                </form>
            </main>
        </>
    );
}
