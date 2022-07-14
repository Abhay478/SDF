import axios from "axios";
// import AuthService from "./auth.service";

const API_URL = "http://localhost:5000/api/lects";
// const user = AuthService.getCurrentUser();

const getAllLects = async (data) => {
    const course_id = data;
  const response = await axios.get(API_URL + '?course_id=' + course_id);
  console.log(response);
  return response;
};

const createLect = async data => {
  const response = await axios.post(API_URL + '/add', data);
  return response;
};


const removeLect = id => {
  return axios.delete(API_URL + `/${id}`);
};

const getLectByTitle = async title => {
    const response = await axios.get('/search?title=' + title);
    return response;
}

// const removeAllCourses = () => {
//   return axios.delete(API_URL + `/courses`);
// };

// const getCourseByTitle = title => {
//   return axios.get(API_URL + `/courses?title=${title}`);
// };

// const getUserSubByID =  (user_id, course_id) => {
//     // console.log(user_id + '   ' + course_id);
//     const response =  axios.get(API_URL + '/subs?user_id=' + user_id + '&course_id=' + course_id);
//     return response;
// }

// const getSubsForCourse = async (course_id) => {
//   return await axios.get(API_URL + '/subs/course?course_id=' + course_id);
// }

const LectService = {
  getAllLects,
  createLect,
  removeLect,
  getLectByTitle
};

export default LectService;
