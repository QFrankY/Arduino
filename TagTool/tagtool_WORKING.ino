// Tagtool sketch by Frank Yu

#define BYTE 0
/* This Portion is not my Code: (c) 2009 Thomas Pintaric (thomas@pintaric.org) 
 * This seems to be a testing procedure for working with sliders
 * Currently not in use

template <typename T>
class double_exponential_filter
{
	public:
		double_exponential_filter(const T _alpha = 1.0) :
			alpha(_alpha),
			has_observations(false)
		{}

		void reset() { has_observations = false; }

		void observe(const T &x)
		{
			if(!has_observations)
			{
				S = x;
				S_prime = x;
				has_observations = true;
			}
			else
			{
				S = (x * alpha) + (S * (1.0-alpha));
				S_prime = (S * alpha) + (S_prime * (1.0-alpha));
			}
		}

		bool can_predict() const
		{
			return(has_observations);
		}

		T predict_t0(const T _min = 0, const T _max = 1023) const
		{
			if(!has_observations) return(0);
			T x0 = (2.0/(1.0-alpha)) * S - (1.0/(1.0-alpha)) * S_prime;
                        if(x0 < _min) x0 = _min;
                        if(x0 > _max) x0 = _max;
	                return(x0);
		}
		
	protected:
		T S, S_prime, alpha;
		bool has_observations;
};

#define DES_ALPHA 0.05f

double_exponential_filter<float> des_slider1(DES_ALPHA);
double_exponential_filter<float> des_slider2(DES_ALPHA);
double_exponential_filter<float> des_slider3(DES_ALPHA);
double_exponential_filter<float> des_slider4(DES_ALPHA);
double_exponential_filter<float> des_slider5(DES_ALPHA);
double_exponential_filter<float> des_slider6(DES_ALPHA);
bool button_pressed = false;

#define SLIDER1 0
#define SLIDER2 1
#define SLIDER3 2
#define SLIDER4 3
#define SLIDER5 4
#define SLIDER6 5

#define BUTTON  7
*/

int xVal, yVal, yPosition, xPosition, slider, pressure = 0;
bool pressed;

/* Recieving information from the touch screen
 * Analog Pins and corresponding touch screen wire:
 * A0 - Red
 * A1 - Green
 * A2 - Yellow
 * A3 - Blue
`*/
void getTouch(){ 
  // Set up the analog pins in preparation for reading the Y value
  pinMode( A1, INPUT ); // Analog pin 1
  pinMode( A3, INPUT ); // Analog pin 3
  pinMode( A0, OUTPUT ); // Analog pin 0
  digitalWrite( A0, LOW ); //  Analog pin 0 as  GND connection
  pinMode( A2, OUTPUT ); // Analog pin 2
  digitalWrite( A2, HIGH ); //  Analog pin 2 as  +5V connection
  yVal = analogRead( 1 ); // Read the Y value

  // Set up the analog pins in preparation for reading the X value
  // from the touchscreen
  pinMode( A0, INPUT ); // Analog pin 0
  pinMode( A2, INPUT ); // Analog pin 2
  pinMode( A1, OUTPUT ); // Analog pin 1
  digitalWrite( A1, LOW ); //  Analog pin 1 as  GND connection
  pinMode( A3, OUTPUT ); // Analog pin 3
  digitalWrite( A3, HIGH ); //  Analog pin 3 as  +5V connection
  xVal = analogRead( 0 ); // Read the x value  

  //Finding Pressure
  //Values do not represent any unit of pressure
  //Experimentally found that when pressure = 1023 -> no touch
  //values with Pressure between 1010 - 1022 are unreliable 
  //Touch = pressure of 998 - 1008
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  pinMode (A3, INPUT);
  digitalWrite (A0, LOW); //Setting as GND
  digitalWrite (A1, HIGH);
  pressure = analogRead (1);
  
}

/* Function used to test Sliders
 * Also not in use
void sample_inputs()
{
  des_slider1.observe((float) analogRead(SLIDER1));
  des_slider2.observe((float) analogRead(SLIDER2));
  des_slider3.observe((float) analogRead(SLIDER3));
  des_slider4.observe((float) analogRead(SLIDER4));
  des_slider5.observe((float) analogRead(SLIDER5));
  des_slider6.observe((float) analogRead(SLIDER6));
  button_pressed = HIGH;
}
  */

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  //sample_inputs();
}


void loop()
{
   if(Serial.available() > 0)// Checking Serial Connection
   {
     byte request = Serial.read(); // Tagtool program will send a request through serial
     if(request == 'A')
     {
       // Used for Sliders
       //Serial.print(readX());
       //Serial.print('\n');
       //Serial.print(readY());
       //Serial.print('\n');
       //delay(10);
     
       getTouch();
        // X range 0 - 915
        // Y range 0 - 800
        // Bottom lefthand corner of touch screen is (0,0)
       
       if (pressure < 1010) {
         yPosition = yVal - 98;
         xPosition = xVal - 55;
       }
       
       /* The follow seven serial prints provides the tagtool program on slider or touch screen information 
        *  In order, the function of these prints are:
        *  1. Color
        *  2. Darkness
        *  3. Brightness
        *  4. Transparency
        *  5. Width
        *  6. Fade
        *  7. Button press
       */

       //Mapping xPosition to a value between 0-255
       Serial.print(xPosition*10/36, BYTE);      
       
       //Checking whether to change darkness, brightness or neither
       //Changing brightness
       if (yPosition < 300){ 
        //Making white easier to click on
          if (yPosition > 45) {
            Serial.print(300-yPosition, BYTE); //More you go down, the higher the brightness value     
          }
          else{
            Serial.print(255, BYTE);
          } 
        Serial.print(0, BYTE);      
       }

       //Changing darkness
       else if (yPosition > 545)
       {
        Serial.print(0, BYTE);      
        Serial.print(yPosition - 545, BYTE);      
       }
       
       //No change to brightness or darkness
       else {
        Serial.print(0, BYTE);      
        Serial.print(0, BYTE); 
       }

       slider = analogRead(4);
       Serial.print(slider/3, BYTE);      

       //functions of tagtool that are untouched
       Serial.print(0, BYTE);      
       Serial.print(0, BYTE); 

       pressed = !digitalRead(2);
       if (pressed) {
        Serial.print(255, BYTE);
       }
       else {
        Serial.print(0, BYTE);
       }
    }// end else if
    
    else if(request == 'N')
    {
      Serial.print("Tagtool.Controller");
    }

    // For use in Arduino IDE to test touchscreen
    else if (request == 'T')
    {
      while (true) { // endless loop to test touchscreen values
        /*getTouch();
  
          if (pressure < 1010) {
            yPosition = yVal - 98;
            xPosition = xVal - 55;
           
            Serial.print("X:");
            Serial.print(xPosition);
            Serial.println();
          
            Serial.print("Y:");
            Serial.print(yPosition);
            Serial.println();
            
            Serial.print("Pressure: ");
            Serial.print(pressure);
            Serial.println();
            delay (500);
          }  */

       slider = analogRead(0);
       Serial.println(slider);
       delay(1000);
          
      }// end while 
    }// end else if
    
  }// end if 

  /* not in use
  else
  {
    sample_inputs();
  }
  */
  
}// end loop()

