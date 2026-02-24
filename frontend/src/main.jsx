import { createRoot } from 'react-dom/client'
import { BrowserRouter, Routes, Route } from 'react-router'
import Home from './pages/Home/Home.jsx'
import Admin from './pages/Admin/Admin.jsx'
import Campuses from './pages/Campuses/Campuses.jsx'
import './index.css'

createRoot(document.getElementById('root')).render(
    <BrowserRouter>
        <Routes>
            <Route path='/' element={<Home />} />
            <Route path='/admin' element={<Admin />} />
            <Route path='/campuses' element={<Campuses />} />
        </Routes>
    </BrowserRouter>
)
