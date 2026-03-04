import { useEffect, useState } from "react";
import axios from "axios";
import HomeButton from "../../../../components/HomeButton/HomeButton";
import "./AddSvnr.css";

export default function AddSvnr() {
    const [schools, setSchools] = useState([]);
    const [selectedSchool, setSelectedSchool] = useState("");
    const [itemName, setItemName] = useState("");
    const [price, setPrice] = useState("");

    useEffect(() => {
        axios.get("/api/University/all")
            .then((res) => {
                const campusNames = res.data.campuses.map(
                    (campus) => campus.name
                );

                setSchools(campusNames);
            })
            .catch((err) => {
                console.error("Error fetching schools:", err);
            });
    }, []);

    const handleSubmit = async (e) => {
        e.preventDefault();

        if (!selectedSchool || !itemName || !price) {
            alert("Please fill out all fields.");
            return;
        }

        try {
            await axios.post(`/api/Souvenir/${selectedSchool}`, {
                name: itemName,
                price: parseFloat(price),
            });

            alert("Souvenir added successfully!");

            // Clear form
            setSelectedSchool("");
            setItemName("");
            setPrice("");
        } catch (err) {
            console.error("Error adding souvenir:", err);
        }
    };

    return (
        <>
            <HomeButton />

            <header data-aos="zoom-in" data-aos-duration="600">
                <h1>Add Souvenir</h1>
            </header>

            <main data-aos="zoom-in" data-aos-duration="600">
                <form onSubmit={handleSubmit} className="add-svnr-form">

                    {/* School Dropdown */}
                    <label>
                        Select School:
                        <select
                            value={selectedSchool}
                            onChange={(e) => setSelectedSchool(e.target.value)}
                        >
                            <option value="">-- Choose a School --</option>
                            {schools.map((schoolName) => (
                                <option key={schoolName} value={schoolName}>
                                    {schoolName}
                                </option>
                            ))}
                        </select>
                    </label>

                    {/* Item Name */}
                    <label>
                        Item Name:
                        <input
                            type="text"
                            value={itemName}
                            onChange={(e) => setItemName(e.target.value)}
                            placeholder="T-Shirt"
                        />
                    </label>

                    {/* Price */}
                    <label>
                        Price:
                        <input
                            type="number"
                            step="0.01"
                            value={price}
                            onChange={(e) => setPrice(e.target.value)}
                            placeholder="19.99"
                        />
                    </label>

                    <button type="submit">Add Souvenir</button>

                </form>
            </main>
        </>
    );
}
