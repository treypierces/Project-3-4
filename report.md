# Report for Project 3

## How does the simulation work?

### Overall idea

For project 3, our group is simulating the Nota Hospital. For this, we have a dataset of doctors, including the time they start and stop work each day and their skill level, and a dataset of patients that will walk in the door to be seen, with a condition level from 1-10.  Patient data is read from a `.csv` dataset into a vector `incoming patients`, while doctors are read from a different `.csv` into `off_duty_doctors`, and also into the doctor B-Tree, `doctor_tree`.

Each time step, doctors check in and out of work based on their shift times. Doctors checking into work are placed in the `available_doctors` priority queue, which is weighted by doctor skill level. A random number of patients walk in the door to the hospital and are added to the `waiting_room` priority queue based on their conditions. From here, doctors and patients are assigned to each other from the top of each of their queues so that the most skilled doctors treat the most hurt patients. When assigned, patients are put into the `patient_tree` B-Tree and both the patient and doctor are updated to show their relation to each other. Patients that have a doctor will start getting better. Every timestep, the patient B-Tree is traversed and patients conditions are decreased relative to their doctor's skill level. 

When a patient gets all better (`condition <= 0`), their doctor is moved back into the `available_doctors` priority queue to get a new patient, and the information linking the two is cleared. In project 4, the patient will be removed from the B-Tree. Similarly, when a doctor clocks out of work, their patient goes back into the `waiting_room` to find a new doctor.

If the hospital runs out of doctors, patients will stay in the `waiting_room` but their `condition` will get 1 higher, indicating that it's getting worse. Patients in the `waiting_room` with `condition` levels of 10 or 11 are considered "in critical condition," and if they make it to level 12 they die.

An example would be a patient coming in with a level 10 condition. They first will be put in the waiting room with top priority. They'll be matched with the best doctor available and be placed in the patient B-tree to receive treatment. After several timesteps, depending on the doctor's skill, the patient's condition will go down to 0 or below, and the patient will be discharged. The patient's doctor will then find a new patient to work on.

The UML class diagram for this prohect is in `Proj3_UML_Diagram.png` in this repository.

### GUI

The GUI displays several useful pieces of information for tracking the workings of the hospital. On the right is a search feature where you can search for doctors and patients by ID number. You can type an ID with the number keys on your keyboard (don't use the number pad). You can type backspace to delete a character. All IDs are 3 digits long. You can switch between looking up a patient and looking up a doctor with 'p' and 'd', respectively. Doctors are all already in the system because they are employees of the hospital. Patients don't get added to the system until they are assigned to a doctor and checked in with the hospital.

On the right side is a graphic showing all the changing information around the hospital. The IDs are shown for which patients just arrived to the waiting room, which doctors left work, which patient was assigned to each doctor, and more. You can use this information to effectively track a doctor's work for the day or to track a patient's progress.

In the bottom left is a button to increase the number of new patients coming in. The hospital usually has enough doctors to cover the patient demand unless this button is pressed. The patient demand increases from a random number below 10 to always be 20 patients at a time. This can overwhelm the hospital and cause patients to start getting into critical conditions.

The time is displayed in the bottom middle, along with the timestep button. Time jumps ahead one hour at a time when the timestep button is pressed.

## B-Tree

The B-Tree, implemented by Trey, has an insert and a search feature, with a delete feature planned for project 4. The tree implements Nodes and Keys to help it keep track of all its data. The t size (minimum number of children) for the B-Trees was set to 10 for both the patients and the doctors. There are 50 doctors and 500 patients, so we felt like this was a good number to use to be able to have sizable nodes and several layers in our B-Tree so we could see the splitting behavior.

## Team contributions

### Trey

- B Tree implementation

### Victoria

- Priority queue
- Data sets
- Time logiv
  - New patients
  - Starting doctors


### Daniel

- Visual representation of simulation with SFML
  - Patient/doctor lookup
- UML Class Diagram
- Time logic
  - Doctor / patient assignments
  - Doctor shifts
  - Patient conditions updates
- Report
