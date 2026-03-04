import { useEffect, useState } from "react";
import axios from "axios";
import HomeButton from "../../../../components/HomeButton/HomeButton";
import "./EditSvnr.css";

export default function EditSvnr() {
    const [schools, setSchools] = useState([]);
    const [selectedSchool, setSelectedSchool] = useState("");

    const [souvenirs, setSouvenirs] = useState([]);
    const [selectedItem, setSelectedItem] = useState("");

    const [newName, setNewName] = useState("");
    const [newPrice, setNewPrice] = useState("");

    // Load schools on page load
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
                setSouvenirs(res.data);
            })
            .catch(err => console.error(err));
    }, [selectedSchool]);

    const handleSubmit = async (e) => {
        e.preventDefault();

        if (!selectedSchool || !selectedItem || !newName || !newPrice) {
            alert("Please fill out all fields.");
            return;
        }

        try {
            await axios.patch(
                `/api/Souvenir/${selectedSchool}`,
                {
                    itemName: selectedItem,
                    newName: newName,
                    price: parseFloat(newPrice)
                }
            );

            alert("Souvenir updated!");

            // Refresh souvenirs list
            const res = await axios.get(`/api/Souvenir/${selectedSchool}`);
            setSouvenirs(res.data);

            // Reset form
            setSelectedItem("");
            setNewName("");
            setNewPrice("");

        } catch (err) {
            console.error(err);
        }
    };

    return (
        <>
            <HomeButton />

            <header data-aos="zoom-in" data-aos-duration="600">
                <h1>Edit Souvenir</h1>
            </header>

            <main data-aos="zoom-in" data-aos-duration="600">
                <form onSubmit={handleSubmit} className="edit-svnr-form">

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
                                onChange={(e) => {
                                    const item = souvenirs.find(
                                        s => s.name === e.target.value
                                    );

                                    setSelectedItem(e.target.value);
                                    setNewName(item?.name || "");
                                    setNewPrice(item?.price || "");
                                }}
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

                    {/* Edit Fields */}
                    {selectedItem && (
                        <>
                            <label>
                                New Name:
                                <input
                                    type="text"
                                    value={newName}
                                    onChange={(e) => setNewName(e.target.value)}
                                />
                            </label>

                            <label>
                                New Price:
                                <input
                                    type="number"
                                    step="0.01"
                                    value={newPrice}
                                    onChange={(e) => setNewPrice(e.target.value)}
                                />
                            </label>

                            <button type="submit">
                                Update Souvenir
                            </button>
                        </>
                    )}

                </form>
            </main>
        </>
    );
}
