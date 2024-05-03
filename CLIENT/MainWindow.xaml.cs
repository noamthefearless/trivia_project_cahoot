using System.Windows;
using System.Media;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using Newtonsoft.Json.Linq;
using System.Xml;
using System;
using System.Collections.Generic;
using System.Windows.Threading;
using System.Reflection.Emit;
using System.Threading;
using System.DirectoryServices.ActiveDirectory;
using System.Threading.Tasks;
using System.Linq;

namespace CLIENT
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public class Stats
        {
            public string userName { get; set; }
            public int score { get; set; }
            public int correctAns { get; set; }
            public int totalAns { get; set; }
            public int totalGames { get; set; }
            public float avgTime { get; set; }
        }


        public class Room
        {
            public int id { get; set; }
            public string name { get; set; }
            public int maxPlayers { get; set; }
            public int currNumOfPlayers { get; set; }
            public int numOfQuestionsInGame { get; set; }
            public int timePerQuestion { get; set; }
            public int isActive { get; set; }
            public List<int> difficulty { get; set; }
            public List<string> category { get; set; }
            public string difficultyStr { get; set; }
            public string categoryStr { get; set; }
            public string isActiveStr { get; set; }
        }

        SoundPlayer player = new SoundPlayer("Assets/kazoot!.wav");


        //codes
        public const int STATUS_POS = 1;
        public const int STATUS_NEG = 0;
        public const int EASY = 1;
        public const int MEDIUM = 2;
        public const int HARD = 3;



        public const byte ERR_RES = 200;

        public const byte LOGIN_REQ = 1;
        public const byte SIGN_UP_REQ = 2;
        public const byte LOGOUT_REQ = 3;
        public const byte HIGH_SCORES_REQ = 6;
        public const byte PERSONAL_STATS_REQ = 7;
        public const byte GET_ROOMS_REQ = 4;
        public const byte JOIN_ROOM_REQ = 8;
        public const byte CREATE_ROOM_REQ = 9;
        public const byte CLOSE_ROOM_REQ = 10;
        public const byte START_GAME_REQ = 11;
        public const byte GET_ROOM_STATE_REQ = 12;
        public const byte LEAVE_ROOM_REQ = 13;
        public const byte LEAVE_GAME_REQ = 14;
        public const byte GET_QUESTION_REQ = 15;
        public const byte SUBMIT_ANSWER_REQ = 16;
        public const byte GET_GAME_RESULTS_REQ = 17;






        public const byte LOGOUT_RES = 103;
        public const byte SIGN_UP_RES = 102;
        public const byte LOGIN_RES = 101;
        public const byte HIGH_SCORES_RES = 106;
        public const byte PERSONAL_STATS_RES = 107;
        public const byte JOIN_ROOM_RESPONSE = 108;
        public const byte GET_ROOMS_RES = 104;
        public const byte CREATE_ROOM_RES = 109;
        public const byte CLOSE_ROOM_RES = 110;
        public const byte START_GAME_RES = 111;
        public const byte GET_ROOM_STATE_RES = 112;
        public const byte LEAVE_ROOM_RES = 113;
        public const byte LEAVE_GAME_RES = 114;
        public const byte GET_QUESTION_RES = 115;
        public const byte SUBMIT_ANSWER_RES = 116;
        public const byte GET_GAME_RESULTS_RES = 117;

        //colors
        Color LoginColor = Colors.LightYellow;
        Color SignUpColor = Colors.LightBlue;
        Color MenuColor = Colors.Purple;
        Color PersonalStatsColor = Colors.Orange;
        Color HighScoresColor = Colors.Silver;
        Color JoinRoomColor = Colors.LightSalmon;
        Color createRoomColor = Colors.GreenYellow;
        Color WaitingRoomColor = Colors.YellowGreen;
        Color GameColor = Colors.Violet;
        Color GameResultsColor = Colors.Fuchsia;


        //booleans
        bool isLogin = true;
        bool isMuted = false;
        bool isPersonal = true;
        bool isAdmin = false;


        //integers
        int MaxPlayersInNewRoom = 0;
        int numOfSecondsPerQuestion = 0;
        int numOfQuestions = 0;
        int dotCounter = 0;
        int time = 0;
        int pointsEarned = 0;

        //communicator
        Communicator comm = new Communicator();


        //timers
        DispatcherTimer refreshMessageSender;
        DispatcherTimer WaitingRoomTitleWriter;
        DispatcherTimer CountDownTimer;
        DispatcherTimer QuestionTimer;
        DispatcherTimer AnswerRecieveTimer;
        DispatcherTimer getResultsTimer;





        //strings
        string answer = "";

        //mutexes
        Mutex ansMu = new Mutex();
        Mutex managingMu = new Mutex();


        //Randoms
        Random rand = new Random();

        //brushes
        SolidColorBrush TransparentB = new SolidColorBrush(Colors.Transparent);
        SolidColorBrush RedB = new SolidColorBrush(Colors.Red);
        SolidColorBrush LimeB = new SolidColorBrush(Colors.Lime);





        public MainWindow()
        {
            InitializeComponent();
            refreshMessageSender = new DispatcherTimer();
            refreshMessageSender.Interval = TimeSpan.FromMilliseconds(100);
            refreshMessageSender.Tick += new EventHandler(refreshRoomState);
            WaitingRoomTitleWriter = new DispatcherTimer();
            WaitingRoomTitleWriter.Interval = TimeSpan.FromMilliseconds(500);
            WaitingRoomTitleWriter.Tick += new EventHandler(WaitingRoomTitleMaker);
            CountDownTimer = new DispatcherTimer();
            CountDownTimer.Interval = TimeSpan.FromSeconds(1);
            CountDownTimer.Tick += new EventHandler(CountDownForNext);
            QuestionTimer = new DispatcherTimer();
            QuestionTimer.Interval = TimeSpan.FromSeconds(1);
            QuestionTimer.Tick += new EventHandler(CountQuestionTime);
            AnswerRecieveTimer = new DispatcherTimer();
            AnswerRecieveTimer.Interval = TimeSpan.FromMilliseconds(100);
            AnswerRecieveTimer.Tick += new EventHandler(RecieveAns);
            getResultsTimer = new DispatcherTimer();
            getResultsTimer.Interval = TimeSpan.FromMilliseconds(100);
            getResultsTimer.Tick += new EventHandler(isResultsObtainable);


            ChangeBackGroundColor(LoginColor);//setting the login color
        }

        /*ChangeBackgroundColor: this function will change the background color
         * input: the color
         * output: non
         */
        private void ChangeBackGroundColor(Color col)
        {
            this.Background = new SolidColorBrush(col);
        }


        /*Quit: this function will close the program
         * input sender object an routed events arguments
         * output non
         */
        private void Quit(object sender, RoutedEventArgs e)
        {
            comm.close();
            this.Close();
        }


        /*LoginOrSignUpChoice: this function will chnge the screnn for the login or sign up
         * input sender object an routed events arguments
         * output: non
         */
        private void LoginOrSignUpChoice(object sender, RoutedEventArgs e)
        {
            if (isLogin)//if login
            {
                emailInput.Visibility = Visibility.Visible;
                EmailIndicator.Visibility = Visibility.Visible;
                resetAllLoginHandlerInputs();
                LoginOrSignUpMsg.Text = "         Sign Up";//changing into sign up
                loginOrSignUpPic.Source = new BitmapImage(new System.Uri("Assets/Sign up.png", System.UriKind.Relative));
                isLoginButton.Content = "Click here to\nlog into an account";
                ChangeBackGroundColor(SignUpColor);
                ErrorLine.Text = "";

            }
            else
            {
                emailInput.Visibility = Visibility.Collapsed;
                EmailIndicator.Visibility = Visibility.Collapsed;
                resetAllLoginHandlerInputs();//changeing into log in
                LoginOrSignUpMsg.Text = "         Login";
                loginOrSignUpPic.Source = new BitmapImage(new System.Uri("Assets/Login.png", System.UriKind.Relative));
                isLoginButton.Content = "Click here\nto create an account";
                ChangeBackGroundColor(LoginColor);
                ErrorLine.Text = "";
            }
            isLogin = !isLogin;
        }


        /*playSong: this function will start playing the song of cahoot
         * input: non
         * output: non
         */
        private void playSong()
        {
            player.PlayLooping();
        }





        /*resetAllLoginHandlerInputs: this function will delete all of the values in the login handler inputs
        * input non
        * output non
        */
        private void resetAllLoginHandlerInputs()
        {
            emailInput.Text = "";
            usernameInput.Text = "";
            passwordInput.Password = "";
        }


        /*changeToMenu: this function will enable only the menu grid and properties
         * input: non
         * output: nom
         */
        private void changeToMenu()
        {
            LoginGrid.Visibility = Visibility.Collapsed;
            MenuGrid.Visibility = Visibility.Visible;
            playSong();
            ToggleSound.Visibility = Visibility.Visible;
            ToggleSoundImage.Visibility = Visibility.Visible;
            ChangeBackGroundColor(MenuColor);
        }


        /*LoginOrSignUp: this function will login or sign up to the game
         * input: sender object an routed events arguments
         * output: non
         */
        private void LoginOrSIgnUp(object sender, RoutedEventArgs e)
        {
            JObject jsonReq = new JObject();
            JObject jsonRes;
            byte code = LOGIN_REQ;
            //checking if requenst can even be sent
            if ((isLogin && usernameInput.Text.Length != 0 && passwordInput.Password.Length != 0) || (!isLogin && usernameInput.Text.Length != 0 && passwordInput.Password.Length != 0 && emailInput.Text.Length != 0))
            {
                jsonReq["username"] = usernameInput.Text;//setting up a default login. if its a sign up it will change
                jsonReq["password"] = passwordInput.Password;
                if (!isLogin)
                {
                    jsonReq["email"] = emailInput.Text;
                    code = SIGN_UP_REQ;
                }
                comm.SendData(code, jsonReq);//sending and getting data
                jsonRes = comm.GetData();
                if ((byte)jsonRes["Code"] == LOGIN_RES)//handeling each scenerio: if login
                {
                    if ((int)jsonRes["status"] == STATUS_POS)
                    {
                        changeToMenu();
                    }
                    else if ((int)jsonRes["status"] == STATUS_NEG)
                    {
                        ErrorLine.Text = "Login faild.\nPlease make sure that the user exists, that the\npassword match\n and that the user is\nnot logged in already";
                    }
                }
                else if ((byte)jsonRes["Code"] == SIGN_UP_RES)//if signUp
                {
                    if ((int)jsonRes["status"] == STATUS_POS)
                    {
                        changeToMenu();
                    }
                    else if ((int)jsonRes["status"] == STATUS_NEG)
                    {
                        ErrorLine.Text = "Sign up faild.\n there is already a user\nwith that username";
                    }
                }
                else if ((byte)jsonRes["Code"] == ERR_RES)//if Error
                {
                    ErrorLine.Text = (string)jsonRes["message"];
                }
            }
            else
            {
                ErrorLine.Text = "1 or more fields\nare not filled";
            }
        }

        /*ToggleSoundButton: this function will toggle the sound button to play or mute
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void ToggleSoundButton(object sender, RoutedEventArgs e)
        {
            if (isMuted)
            {
                playSong();//in case of play
                ToggleSoundImage.Source = new BitmapImage(new Uri("Assets/PlaySong.png", System.UriKind.Relative));
            }
            else
            {
                player.Stop();//in case of mute
                ToggleSoundImage.Source = new BitmapImage(new Uri("Assets/MuteSong.png", System.UriKind.Relative));
            }
            isMuted = !isMuted;
        }

        /*changeToLogin: this function will change the gui into the login grid
         * input: nothing
         * output: non
         */
        private void changeToLogin()
        {
            LoginGrid.Visibility = Visibility.Visible;
            MenuGrid.Visibility = Visibility.Collapsed;//closing grid
            player.Stop();//there is no music in login
            isMuted = false;
            ToggleSound.Visibility = Visibility.Collapsed;
            ToggleSoundImage.Source = new BitmapImage(new Uri("Assets/PlaySong.png", System.UriKind.Relative));//setting up for next login
            ToggleSoundImage.Visibility = Visibility.Collapsed;
            if (isLogin)//changeing to right color
            {
                ChangeBackGroundColor(LoginColor);
            }
            else
            {
                ChangeBackGroundColor(SignUpColor);
            }
            resetAllLoginHandlerInputs();//reseting inputs
            ErrorLine.Text = "";

        }

        /*Logout: this function will log out of menu
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void Logout(object sender, RoutedEventArgs e)
        {
            JObject j = new JObject();
            comm.SendData(LOGOUT_REQ, new JObject());
            j = comm.GetData();
            if ((byte)j["Code"] == LOGOUT_RES && (int)j["status"] == STATUS_POS)// checking if the server logged you out
            {
                changeToLogin();
            }
        }

        /*GoToStatistics: this function will go to the statistics from the man menu
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void GoToStatistics(object sender, RoutedEventArgs e)
        {
            string statsOutput = "";
            JObject json = new JObject();
            Stats stat;
            List<Stats> stats;
            //closing the menu
            MenuGrid.Visibility = Visibility.Collapsed;
            StatisticsGrid.Visibility = Visibility.Visible;
            isPersonal = false;
            PersonalStatsOrHighScoresChoice(new object(), new RoutedEventArgs());
            //getting Personal Data
            comm.SendData(PERSONAL_STATS_REQ, new JObject());
            json = comm.GetData();
            if ((byte)json["Code"] == PERSONAL_STATS_RES && (int)json["status"] == STATUS_POS)
            {
                stat = json["statistics"].ToObject<Stats>();
                statsOutput = stat.userName + "'s stats:\nScore: " + Convert.ToString(stat.score) + "\nNumber of correct answers: " + Convert.ToString(stat.correctAns) + "\nNumber of total answers: " + Convert.ToString(stat.totalAns) + "\nNumber of total games: " + Convert.ToString(stat.totalGames) + "\nAverarge answer time: " + stat.avgTime.ToString("F2");
                personalStats.Text = statsOutput;//putting the personal stats in the textBlock
            }

            //getting high scores:
            comm.SendData(HIGH_SCORES_REQ, new JObject());
            json = comm.GetData();
            if ((byte)json["Code"] == HIGH_SCORES_RES && (int)json["status"] == STATUS_POS)
            {
                stats = json["statistics"].ToObject<List<Stats>>();
                HighScoresTable.ItemsSource = stats;
            }

        }

        /*Return To menu: this function will return to the menu and resert all of the options in all of the sub menu grids
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void ReturnToMenu(object sender, RoutedEventArgs e)
        {
            JObject json = new JObject();
            MenuGrid.Visibility = Visibility.Visible;
            ChangeBackGroundColor(MenuColor);
            //reseting statistics grid
            StatisticsGrid.Visibility = Visibility.Collapsed;
            JoinRoomGrid.Visibility = Visibility.Collapsed;
            CreateRoomGrid.Visibility = Visibility.Collapsed;
            if (WaitingRoomGrid.Visibility == Visibility.Visible)
            {
                WaitingRoomGrid.Visibility = Visibility.Collapsed;
                refreshMessageSender.Stop();
                Thread.Sleep(101);
                if (isAdmin)
                {
                    comm.SendData(CLOSE_ROOM_REQ, new JObject());
                }
                else
                {
                    comm.SendData(LEAVE_ROOM_REQ, new JObject());
                }
                json = comm.GetData();
                if ((byte)json["Code"] != ERR_RES && (int)json["status"] == STATUS_POS)
                {
                    changeMusic("Assets\\kazoot!.wav");//doing so, so that i would know when something went wrong, because the music is the same
                }
            }
            else if (GameGrid.Visibility == Visibility.Visible || GameResultsGrid.Visibility == Visibility.Visible)
            {
                GameGrid.Visibility = Visibility.Collapsed;
                GameResultsGrid.Visibility = Visibility.Collapsed;
              
                comm.SendData(LEAVE_GAME_REQ, new JObject());
                json = comm.GetData();
                if ((byte)json["Code"] != ERR_RES && (int)json["status"] == STATUS_POS)
                {
                    changeMusic("Assets\\kazoot!.wav");//doing so, so that i would know when something went wrong, because the music is the same
                }
            }

        }

        /*PersonalStatsOrHighScoresChoice: this function will rotate between the personal score, and the high scores screens
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void PersonalStatsOrHighScoresChoice(object sender, RoutedEventArgs e)
        {
            if (isPersonal)// if the change is into the high scores
            {
                HighScoresTableScroller.Visibility = Visibility.Visible;
                HighScoresTable.Visibility = Visibility.Visible;
                personalStats.Visibility = Visibility.Collapsed;
                StatsDecor1.Source = new BitmapImage(new Uri("Assets\\MedalDecor.png", System.UriKind.Relative));
                StatsDecor2.Source = new BitmapImage(new Uri("Assets\\MedalDecor.png", System.UriKind.Relative));
                switchBetweenPersonalStatsAndHighScoresImage.Source = new BitmapImage(new Uri("Assets\\Personal stats.png", System.UriKind.Relative));
                ChangeBackGroundColor(HighScoresColor);
            }
            else//if the change is into the personal scores
            {
                HighScoresTableScroller.Visibility = Visibility.Collapsed;
                HighScoresTable.Visibility = Visibility.Collapsed;
                personalStats.Visibility = Visibility.Visible;
                StatsDecor1.Source = new BitmapImage(new Uri("Assets\\BullsEyeDecor.png", System.UriKind.Relative));
                StatsDecor2.Source = new BitmapImage(new Uri("Assets\\BullsEyeDecor.png", System.UriKind.Relative));
                switchBetweenPersonalStatsAndHighScoresImage.Source = new BitmapImage(new Uri("Assets\\High Scores.png", System.UriKind.Relative));
                ChangeBackGroundColor(PersonalStatsColor);
            }
            isPersonal = !isPersonal;
        }

        /*GoTOJoinRoom: this function will load the Join Room screen
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void GoToJoinRoom(object sender, RoutedEventArgs e)
        {
            JObject json = new JObject();
            List<Room> rooms;



            MenuGrid.Visibility = Visibility.Collapsed;
            JoinRoomGrid.Visibility = Visibility.Visible;
            JoinRoomErrorLine.Text = "";
            IdTextBox.Text = "";
            ChangeBackGroundColor(JoinRoomColor);//setting the grid itself


            comm.SendData(GET_ROOMS_REQ, new JObject());// getting rooms
            json = comm.GetData();
            if ((byte)json["Code"] == GET_ROOMS_RES)
            {
                rooms = json["rooms"].ToObject<List<Room>>();
                foreach (Room room in rooms)
                {
                    // Convert the categories list to a string with commas
                    room.categoryStr = string.Join(",", room.category);
                    room.difficultyStr = string.Join(",", room.difficulty);
                    room.difficultyStr = room.difficultyStr.Replace("1", "Easy");
                    room.difficultyStr = room.difficultyStr.Replace("2", "Medium");
                    room.difficultyStr = room.difficultyStr.Replace("3", "Hard");


                    if (room.isActive == 0)
                    {
                        room.isActiveStr = "Inactive";
                    }
                    else
                    {
                        room.isActiveStr = "Active";
                    }
                }
                AllRoomsTable.ItemsSource = rooms;//setting datagrid
            }
        }

        /*JoinIntoRoom: this function will join into a room 
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void JoinIntoRoom(object sender, RoutedEventArgs e)
        {
            JObject json = new JObject();

            json = new JObject();
            int Id = 0;
            if (Int32.TryParse(IdTextBox.Text, out Id))//trying to turn the string to int
            {
                json["id"] = Id;
                comm.SendData(JOIN_ROOM_REQ, json);
                json = comm.GetData();
                if ((byte)json["Code"] == JOIN_ROOM_RESPONSE && (int)json["status"] == STATUS_NEG)//if not possible
                {
                    JoinRoomErrorLine.Text = "room is either full, or not exists";
                }
                else if ((byte)json["Code"] == ERR_RES)//if error
                {
                    JoinRoomErrorLine.Text = "Id is invalid!";
                }
                else if ((byte)json["Code"] == JOIN_ROOM_RESPONSE && (int)json["status"] == STATUS_POS)//if possible
                {
                    JoinRoomErrorLine.Text = "";
                    isAdmin = false;
                    changeToWaitingRoom();
                }
            }
            else
            {
                JoinRoomErrorLine.Text = "Id is invalid!";
            }

        }


        /*GoToCreateRoom: this function will go to the create room from the menu
         * input: the sender object and the routed events arguments
         * output: non
         */
        private void GoToCreateRoom(object sender, RoutedEventArgs e)
        {
            ChangeBackGroundColor(createRoomColor);
            MenuGrid.Visibility = Visibility.Collapsed;
            CreateRoomGrid.Visibility = Visibility.Visible;
            //setting checkboxes to false
            IsEasy.IsChecked = false;
            IsMedium.IsChecked = false;
            IsHard.IsChecked = false;
            IsFilms.IsChecked = false;
            IsVideoGames.IsChecked = false;
            IsMythology.IsChecked = false;
            IsTelevision.IsChecked = false;
            IsHistory.IsChecked = false;
            IsSports.IsChecked = false;
            IsGeneralKnowledge.IsChecked = false;
            //setting minimal values
            numOfQuestions = 5;
            numOfSecondsPerQuestion = 10;
            MaxPlayersInNewRoom = 1;
            roomNameInput.Text = "";
            CreateRoomErrorLine.Text = "";

            numberOfSecondsPerQuestionDisplay.Text = "Question time\n" + Convert.ToString(numOfSecondsPerQuestion);
            numberOfPlayersDisplay.Text = "Max players\n" + Convert.ToString(MaxPlayersInNewRoom);
            numberOfQuestionDisplay.Text = "Question count\n" + Convert.ToString(numOfQuestions);
        }


        /*incMaxPlayer: this function will increase the max number of players in the new room
        * input: the sender object and the routed events arguments
        * output: non
        */
        private void incMaxPlayers(object sender, RoutedEventArgs e)
        {
            if (MaxPlayersInNewRoom < 8)
            {
                MaxPlayersInNewRoom++;
                numberOfPlayersDisplay.Text = "Max players\n" + Convert.ToString(MaxPlayersInNewRoom);//setting to screen
            }
        }


        /*decMaxPlayer: this function will decrease the max number of players in the new room
        * input: the sender object and the routed events arguments
        * output: non
        */
        private void decMaxPlayers(object sender, RoutedEventArgs e)
        {
            if (MaxPlayersInNewRoom > 1)
            {
                MaxPlayersInNewRoom--;
                numberOfPlayersDisplay.Text = "Max players\n" + Convert.ToString(MaxPlayersInNewRoom);//setting to screen
            }
        }
        /*incQuestionNum: this function will increase the max number of questions in the new room
        * input: the sender object and the routed events arguments
        * output: non
        */
        private void incQuestionNum(object sender, RoutedEventArgs e)
        {
            if (numOfQuestions < 50)
            {
                numOfQuestions++;
                numberOfQuestionDisplay.Text = "Question count\n" + Convert.ToString(numOfQuestions);
            }
        }
        /*decQuestionNum: this function will decrease the max number of questions in the new room
        * input: the sender object and the routed events arguments
        * output: non
        */
        private void decQuestionNum(object sender, RoutedEventArgs e)
        {
            if (numOfQuestions > 5)
            {
                numOfQuestions--;
                numberOfQuestionDisplay.Text = "Question count\n" + Convert.ToString(numOfQuestions);
            }
        }



        /*incQuestionTime: this function will increase the time per question in the new room
        * input: the sender object and the routed events arguments
        * output: non
*       */
        private void incQuestionTime(object sender, RoutedEventArgs e)
        {
            if (numOfSecondsPerQuestion < 60)
            {
                numOfSecondsPerQuestion++;
                numberOfSecondsPerQuestionDisplay.Text = "Question time\n" + Convert.ToString(numOfSecondsPerQuestion);
            }
        }

        /*decQuestionTime: this function will decrease the time per question in the new room
        * input: the sender object and the routed events arguments
        * output: non
        *       */
        private void decQuestionTime(object sender, RoutedEventArgs e)
        {
            if (numOfSecondsPerQuestion > 10)
            {
                numOfSecondsPerQuestion--;
                numberOfSecondsPerQuestionDisplay.Text = "Question time\n" + Convert.ToString(numOfSecondsPerQuestion);
            }
        }


        /*CreateNewRoom: this function will CreateNewRoom
        * input: the sender object and the routed events arguments
        * output: non
*       */
        private void CreateNewRoom(object sender, RoutedEventArgs e)
        {
            JObject jToSend = new JObject();
            JObject ans = new JObject();
            var categories = new JArray();
            var difficulties = new JArray();
            if ((IsEasy.IsChecked == false && IsHard.IsChecked == false && IsMedium.IsChecked == false) || (IsFilms.IsChecked == false && IsVideoGames.IsChecked == false && IsTelevision.IsChecked == false && IsHistory.IsChecked == false && IsMythology.IsChecked == false && IsSports.IsChecked == false && IsGeneralKnowledge.IsChecked == false) || (roomNameInput.Text == ""))
            {
                CreateRoomErrorLine.Text = "one or more fields are not filled";// printing error
            }
            else
            {
                jToSend["name"] = roomNameInput.Text;//setting values in json
                jToSend["numOfQuestionsInGame"] = numOfQuestions;
                jToSend["maxPlayers"] = MaxPlayersInNewRoom;
                jToSend["timePerQuestion"] = numOfSecondsPerQuestion;
                //these ifs will add the difficulties
                if (IsEasy.IsChecked == true)
                {
                    difficulties.Add(EASY);
                }

                if (IsMedium.IsChecked == true)
                {
                    difficulties.Add(MEDIUM);
                }

                if (IsHard.IsChecked == true)
                {
                    difficulties.Add(HARD);
                }


                jToSend["difficulties"] = difficulties;
                //these ifs will add the categories
                if (IsFilms.IsChecked == true)
                {
                    categories.Add("Films");
                }

                if (IsVideoGames.IsChecked == true)
                {
                    categories.Add("Video Games");
                }

                if (IsTelevision.IsChecked == true)
                {
                    categories.Add("Television");
                }

                if (IsHistory.IsChecked == true)
                {
                    categories.Add("History");
                }

                if (IsMythology.IsChecked == true)
                {
                    categories.Add("Mythology");
                }
                if (IsSports.IsChecked == true)
                {
                    categories.Add("Sports");
                }
                if (IsGeneralKnowledge.IsChecked == true)
                {
                    categories.Add("General Knowledge");
                }

                jToSend["categories"] = categories;
                comm.SendData(CREATE_ROOM_REQ, jToSend);
                ans = comm.GetData();
                if ((byte)ans["Code"] == CREATE_ROOM_RES && (int)ans["status"] == STATUS_POS)
                {
                    CreateRoomErrorLine.Text = "";
                    isAdmin = true;
                    changeToWaitingRoom();



                }
            }

        }


        /*changeMusic: this function will change the music
         * input: the path to new music
         * output: non
         */
        public void changeMusic(string path)
        {
            player.Stop();
            player.SoundLocation = path;
            if (!isMuted)
            {
                playSong();
            }
        }
        /*changeToWaitinRoom: this function will go to the Waiting room grid
         * input: non
         * output: non
         */
        public void changeToWaitingRoom()
        {
            CreateRoomGrid.Visibility = Visibility.Collapsed;//going to wating room
            JoinRoomGrid.Visibility = Visibility.Collapsed;
            WaitingRoomGrid.Visibility = Visibility.Visible;

            WaitingRoomTitle.Text = "Waiting for the Admin\nto start the game";
            ChangeBackGroundColor(WaitingRoomColor);
            changeMusic("Assets\\SaxKahoot.wav");
            //setting timers


            //checking if this is admin, and he needs a begin game button
            if (isAdmin)
            {
                BeginGameButton.Visibility = Visibility.Visible;
            }
            else
            {
                BeginGameButton.Visibility = Visibility.Collapsed;
            }
            ReturnToMenuFromWaitingRoom.Visibility = Visibility.Visible;
            RoomState.FontSize = 25;
            CountDownToGame.Text = "";
            refreshMessageSender.Start();//starting to refresh every 100 milisecondes
            WaitingRoomTitleWriter.Start();//starting to write the title


        }


        /*refreshRoomState: this function will refresh the Waiting roo data
         * input: the sender and the event arguments
         * output: non
         */
        private void refreshRoomState(object sender, EventArgs e)
        {
            JObject response = new JObject();
            string players;
            comm.SendData(GET_ROOM_STATE_REQ, new JObject());
            response = comm.GetData();
            if ((byte)response["Code"] == GET_ROOM_STATE_RES && (int)response["status"] == STATUS_POS && (int)response["hasGameBegun"] == STATUS_NEG)//If the game hasn't begun yet
            {
                players = String.Join("\n", response["players"].Values<string>());//getting users
                //the admin is always first
                RoomState.Text = "Players:\n" + players + "\n\nAdmin: " + (string)response["players"][0] + "\nNumber of questions: " + ((int)response["questionCount"]).ToString() + "\nTime for each question: " + ((int)response["answerTimeOut"]).ToString() + " seconds";

            }
            else if ((byte)response["Code"] == GET_ROOM_STATE_RES && (int)response["status"] == STATUS_NEG)//If the admin has left
            {
                RoomState.FontSize = 35;//in case the admin left
                refreshMessageSender.Stop();
                WaitingRoomTitleWriter.Stop();


                RoomState.Text = "\n\nLooks like the Admin left.\nClick on the Leave room button\nto return to the main menu";
                WaitingRoomTitle.Text = "Admin left...";
            }
            else if ((byte)response["Code"] == GET_ROOM_STATE_RES && (int)response["status"] == STATUS_POS && (int)response["hasGameBegun"] == STATUS_POS)//If the game has started
            {
                refreshMessageSender.Stop();
                WaitingRoomTitleWriter.Stop();
                RoomState.FontSize = 35;//in case the admin left
                startGameCountdown();
            }
        }



        /*WaitingRoomTitleMAker: this function will make the 3 dots in the title
        * input: the sender and the event arguments
        * output: non
        */
        private void WaitingRoomTitleMaker(object sender, EventArgs e)
        {
            if (dotCounter <= 2)
            {
                WaitingRoomTitle.Text += ".";
                waitingForAllPlayersToFinishHolder.Text += ".";

                dotCounter++;
            }
            else
            {
                WaitingRoomTitle.Text = "Waiting for the Admin\n to start the game";//removing back to normal
                waitingForAllPlayersToFinishHolder.Text = "Game is not\nFinished!\n\n\nWaiting for all players to finish their game";

                dotCounter = 0;
            }

        }



        /*BeginGame: this function will start the game
         * input: the sender object and the routed eventes arguments
         * output: non
         */
        private void BeginGame(object sender, RoutedEventArgs e)
        {
            JObject json = new JObject();
            refreshMessageSender.Stop();
            WaitingRoomTitleWriter.Stop();//stopping timers
            comm.SendData(START_GAME_REQ, new JObject());
            json = comm.GetData();
            if ((byte)json["Code"] == START_GAME_RES && (int)json["status"] == STATUS_POS)// do something when game started
            {

                startGameCountdown();
            }
        }





        /*
         *startGameCountdown: this function will start the game countdown 
         * input: non
         * output: non
         */
        private async void startGameCountdown()
        {
            refreshMessageSender.Stop();
            WaitingRoomTitleWriter.Stop();//stopping timers
            await Task.Delay(201);
            BeginGameButton.Visibility = Visibility.Collapsed;
            ReturnToMenuFromWaitingRoom.Visibility = Visibility.Collapsed;
            pointsEarned = 0;
            pointsHolder.Text = "0";
            RoomState.Text = "";
            WaitingRoomTitle.Text = "Game starts in:";
            time = 5;
            timeHolder.Text = time.ToString();
            CountDownToGame.Text = time.ToString();
            CountDownTimer.Start();//starting countdown
            changeMusic("Assets\\GameMusic.wav");


        }

        /*setAnswers: this function will randomally assign the answer buttons with values
         * input: the 4 possible answers
         * output: non
         */
        private void setAnswers(string ans1, string ans2, string ans3, string ans4)
        {
            List<string> answers = new List<string>() { ans1, ans2, ans3, ans4 };
            List<string> answersRandomized = new List<string>();
            List<int> alreadyChosen = new List<int>();
            int randoNum = 0;
            randoNum = rand.Next(0, 4);
            do
            {
                while (alreadyChosen.Contains(randoNum))//making sure that the random number is new
                {
                    randoNum = rand.Next(0, 4);
                }
                answersRandomized.Add(answers[randoNum]);
                alreadyChosen.Add(randoNum);

            } while (alreadyChosen.Count < 4);
            answer1buttonText.Text = answersRandomized[0];//setting values in random
            answer2buttonText.Text = answersRandomized[1];
            answer3buttonText.Text = answersRandomized[2];
            answer4buttonText.Text = answersRandomized[3];
        }


        /*setDifficculty: this function will set the difficulty in the difficulty holder
        * input: the difficulty
        * output: non
        */
        private void setDifficulty(int diff)
        {
            if (diff == HARD)
            {
                QuestionDifficultyHolder.Text = "Hard";
            }
            else if (diff == MEDIUM)
            {
                QuestionDifficultyHolder.Text = "Medium";
            }
            else if (diff == EASY)
            {
                QuestionDifficultyHolder.Text = "Easy";
            }
        }







        /*showCorrectAns: this function will set colors to the answers. the correct one will be green lime, and the others will be red
         *input: the correct ans 
         * output: non
         */
        private void showCorrectAns(string correctAns)
        {
            if (answer1buttonText.Text == correctAns)// checking what button has the correct answer, and setting the right colors from there
            {
                answer1buttonText.Background = LimeB;
                answer2buttonText.Background = RedB;
                answer3buttonText.Background = RedB;
                answer4buttonText.Background = RedB;
            }
            else if (answer2buttonText.Text == correctAns)
            {
                answer2buttonText.Background = LimeB;
                answer3buttonText.Background = RedB;
                answer4buttonText.Background = RedB;
                answer1buttonText.Background = RedB;
            }
            else if (answer3buttonText.Text == correctAns)
            {
                answer3buttonText.Background = LimeB;
                answer4buttonText.Background = RedB;
                answer1buttonText.Background = RedB;
                answer2buttonText.Background = RedB;
            }
            else if (answer4buttonText.Text == correctAns)
            {
                answer4buttonText.Background = LimeB;
                answer1buttonText.Background = RedB;
                answer2buttonText.Background = RedB;
                answer3buttonText.Background = RedB;
            }
        }



        /*isLastQuestion: this function will return true if this was the last question
         * input:non
         * output: a bool
         */
        private bool isLastQuestion()
        {
            bool result = false;
            List<string> howManyLeft = QuestionNumberHolder.Text.Split("/").ToList();
            if (howManyLeft[0] == howManyLeft[1])//checking if this question was the last
            {
                result = true;  
            }
            

            return result;
        }





        /*manageGame: this function will set the current question and the waiting time, as well as submmiting the answers
         * input: a bool, to know what action to do
         * output: non
         */
        private async void manageGame(bool isGetQuestion)
        {
            managingMu.WaitOne();
            WaitingRoomGrid.Visibility = Visibility.Collapsed;
            GameGrid.Visibility = Visibility.Visible;
            ChangeBackGroundColor(GameColor);
            JObject json = new JObject();
            if (isGetQuestion == true)// in case of get Question
            {
                CountDownTimer.Stop();
                await Task.Delay(101);
                comm.SendData(GET_QUESTION_REQ, new JObject());
                json = comm.GetData();
                if ((byte)json["Code"] == GET_QUESTION_RES && (int)json["status"] == STATUS_POS)// if there are any questions left
                {
                    answer1buttonText.Background = TransparentB;
                    answer2buttonText.Background = TransparentB;
                    answer3buttonText.Background = TransparentB;
                    answer4buttonText.Background = TransparentB;



                    QuestionShower.FontSize = 35;

                    timeIndiCator.Text = "Time Left:";//setting the time for the question
                    time = (int)json["timeForQuestion"];
                    timeHolder.Text = time.ToString();


                    ansMu.WaitOne();
                    answer = "";// resetting answer to no answer
                    ansMu.ReleaseMutex();
                    QuestionShower.Text = (string)json["question"]["question"];//placing all values
                    setAnswers((string)json["question"]["falseAns1"], (string)json["question"]["falseAns2"], (string)json["question"]["falseAns3"], (string)json["question"]["correctAns"]);
                    setDifficulty((int)json["question"]["difficulty"]);
                    QuestionNumberHolder.Text = (((int)json["questionIndex"]) + 1).ToString() + "/" + ((int)json["numOfQuestions"]).ToString();//setting question number

                    AnswerRecieveTimer.Start();// starting question
                    QuestionTimer.Start();
                }
                else if ((byte)json["Code"] == GET_QUESTION_RES && (int)json["status"] == STATUS_NEG)//if the questions are over
                {
                    GameGrid.Visibility = Visibility.Collapsed;
                    GameResultsGrid.Visibility = Visibility.Visible;
                    //collapsing all of the unneeded things in the results grid
                    LaveGameResultsButton.Visibility = Visibility.Collapsed;
                    ThirdPlacePodium.Visibility = Visibility.Collapsed;
                    SecondPlacePodium.Visibility = Visibility.Collapsed;
                    FirstPlacePodium.Visibility = Visibility.Collapsed;
                    ThirdPlacePodiumIndiCator.Visibility = Visibility.Collapsed;
                    SecondPlacePodiumIndiCator.Visibility = Visibility.Collapsed;
                    FirstPlacePodiumIndiCator.Visibility = Visibility.Collapsed;
                    ThirdPlacePodiumName.Visibility = Visibility.Collapsed;
                    SecondPlacePodiumName.Visibility = Visibility.Collapsed;
                    FirstPlacePodiumName.Visibility = Visibility.Collapsed;

                    waitingForAllPlayersToFinishHolder.Visibility = Visibility.Visible;
                    ChangeBackGroundColor(GameResultsColor);
                    changeMusic("Assets\\ResultsMusic.wav");
                    WaitingRoomTitleWriter.Start();
                    getResultsTimer.Start();
                }   
                
            }
            else 
            {
                AnswerRecieveTimer.Stop();
                await Task.Delay(101);
                QuestionTimer.Stop();
                await Task.Delay(101);// these 4 lines will stop the timers
                ansMu.WaitOne();
                comm.SendData(SUBMIT_ANSWER_REQ, new JObject { { "answer", answer } });
                ansMu.ReleaseMutex();
                json = comm.GetData();//sending answer
                if ((byte)json["Code"] == SUBMIT_ANSWER_RES && (int)json["status"] == STATUS_POS)
                {
                    QuestionShower.FontSize = 65;
                    time = 5;
                    timeHolder.Text = time.ToString();//setting time
                    if ((int)json["points"] == 0)//checking if right or wrong
                    {
                        QuestionShower.Text = "WRONG!";
                    }
                    else 
                    {
                        QuestionShower.Text = "CORRECT!\n+" + ((int)json["points"]).ToString() + " points!";
                        pointsEarned += (int)json["points"];
                    }
                    StreakIndiCator.Text = "Streak:";//setting streak
                    StreakHolder.Text = ((int)json["streak"]).ToString();
                    if (!isLastQuestion())
                    {
                        timeIndiCator.Text = "Next Question In:";
                    }
                    else
                    {
                        timeIndiCator.Text = "Game Results In:";

                    }
                    showCorrectAns((string)json["correctAns"]);
                    time = 6;
                    pointsHolder.Text = pointsEarned.ToString();
                    CountDownTimer.Start();

                }


            }
            managingMu.ReleaseMutex();
        }
        


        /*RecieveAns: this function will call the managerFunction once the answer has been selected
        * input: the sender and the event arguments
        * output: non
        */
        private void RecieveAns(object sender, EventArgs e)
        {
            ansMu.WaitOne();
            if (answer != "")
            {
                AnswerRecieveTimer.Stop();
                manageGame(false);
            }
            ansMu.ReleaseMutex();
        }










        /*CountQuestionTime: this function will count the seconds for the question
        * input: the sender and the event arguments
        * output: non
        */
        private void CountQuestionTime(object sender, EventArgs e)
        {
            time--;
            if (time < 0)
            {
                QuestionTimer.Stop();
                manageGame(false);
            }
            else
            {
                timeHolder.Text = time.ToString();
            }
        }

        /*CountDownForNext: this function will count 5 seconds before the next question
         * input: the sender and the event arguments
        * output: non
        */
        private void CountDownForNext(object sender, EventArgs e)
        {
            time--;
            if (time < 0)
            {
                CountDownTimer.Stop();
                manageGame(true);
            }
            else
            {
                CountDownToGame.Text = time.ToString();
                timeHolder.Text = time.ToString();
            }
        }





        /*isResultsObtainable: this function will check if the results can be obtained every 1/10 of a second 
         * input: the sender and the event arguments
        * output: non
        */
        private void isResultsObtainable(object sender, EventArgs e)
        {
            JObject json = new JObject();
            comm.SendData(GET_GAME_RESULTS_REQ, new JObject());
            json = comm.GetData();
            if ((byte)json["Code"] == GET_GAME_RESULTS_RES && (int)json["status"] == STATUS_POS)
            {
                LaveGameResultsButton.Visibility = Visibility.Visible;
                waitingForAllPlayersToFinishHolder.Visibility = Visibility.Collapsed;
                setWinners(json);




                WaitingRoomTitleWriter.Stop();
                getResultsTimer.Stop();
            }
        }


        /*setWinners: this function will set the winners of the game on the podium
         * input: the json of the winners
         * output: non
         */
        private void setWinners(JObject json)
        {
            JArray results = (JArray)json["results"];//extraction results
            LaveGameResultsButton.Visibility = Visibility.Visible;//setting poidum to visable
            ThirdPlacePodium.Visibility = Visibility.Visible;
            SecondPlacePodium.Visibility = Visibility.Visible;
            FirstPlacePodium.Visibility = Visibility.Visible;
            ThirdPlacePodiumIndiCator.Visibility = Visibility.Visible;
            SecondPlacePodiumIndiCator.Visibility = Visibility.Visible;
            FirstPlacePodiumIndiCator.Visibility = Visibility.Visible;
            ThirdPlacePodiumName.Visibility = Visibility.Visible;
            SecondPlacePodiumName.Visibility = Visibility.Visible;
            FirstPlacePodiumName.Visibility = Visibility.Visible;

            //setting default values
            ThirdPlacePodium.Text = "Score:\n";
            SecondPlacePodium.Text = "Score:\n";
            FirstPlacePodium.Text = "Score:\n";

            ThirdPlacePodiumName.Text = "";
            SecondPlacePodiumName.Text = "";
            FirstPlacePodiumName.Text = "";



            if (results.Count > 0)//checkoing for first, second, and third place, and then setting the name and score
            {
                FirstPlacePodium.Text = "Score:\n" + ((int)results[0]["points"]).ToString();
                FirstPlacePodiumName.Text = (string)results[0]["userName"];
            }
            if (results.Count > 1)
            {
                SecondPlacePodium.Text = "Score:\n" + ((int)results[1]["points"]).ToString();
                SecondPlacePodiumName.Text = (string)results[1]["userName"];
            }
            if (results.Count > 2)
            {
                ThirdPlacePodium.Text = "Score:\n" + ((int)results[2]["points"]).ToString();
                ThirdPlacePodiumName.Text = (string)results[2]["userName"];
            }

        }




















        /*the next 4 function will set the answer to the answer var to send to the server
         * 
         * 
         * 
         */
        private void answer1button_Click(object sender, RoutedEventArgs e)
        {
            ansMu.WaitOne();
            answer = answer1buttonText.Text;
            ansMu.ReleaseMutex();
        }

        private void answer2button_Click(object sender, RoutedEventArgs e)
        {
            ansMu.WaitOne();
            answer = answer2buttonText.Text;
            ansMu.ReleaseMutex();
        }

        private void answer3button_Click(object sender, RoutedEventArgs e)
        {
            ansMu.WaitOne();
            answer = answer3buttonText.Text;
            ansMu.ReleaseMutex();
        }

        private void answer4button_Click(object sender, RoutedEventArgs e)
        {
            ansMu.WaitOne();
            answer = answer4buttonText.Text;
            ansMu.ReleaseMutex();
        }





        /*LeaveGame this function will return to the menu from a game
         * input: non
         * output: non
         */
        private void LaveGame(object sender, RoutedEventArgs e)
        {
            managingMu.WaitOne();
            QuestionTimer.Stop();
            AnswerRecieveTimer.Stop();
            CountDownTimer.Stop();//stopping all possible Timers
            managingMu.ReleaseMutex();
            ReturnToMenu(new object(), new RoutedEventArgs());
        }
    }
}






