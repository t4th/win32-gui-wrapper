#include "thWin32App.h"

#include <functional>
#include <array>
#include <stdexcept>

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")

// Simple example calculator implementation.
// Operates on 32 ints. No under/overflow checks.
// todo: maybe add +/- sign button.
class CalculatorLogic
{
    private:
        enum class Operation
        {
            None, Add, Sub, Mul, Div
        };

    public:
        void clear()
        {
            m_operation = Operation::None;
            m_previous_value = 0;
            m_dividedByZero = false;
        }

        void add( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Add;
        }

        void minus( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Sub;
        }

        void multiply( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Mul;
        }

        void divide( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Div;
        }

        bool result( int a_current_value, int & a_result)
        {
            switch ( m_operation)
            {
                case Operation::Add:
                {
                    a_result = m_previous_value + a_current_value;
                    break;
                }
                case Operation::Sub:
                {
                    a_result = m_previous_value - a_current_value;
                    break;
                }
                case Operation::Mul:
                {
                    a_result = m_previous_value * a_current_value;
                    break;
                }
                case Operation::Div:
                {
                    if ( 0 == a_current_value)
                    {
                        m_dividedByZero = true;
                        m_operation = Operation::None;
                        return false;
                    }

                    a_result = m_previous_value / a_current_value;
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool isDivByZero()
        {
            return m_dividedByZero;
        }

    private:
        Operation   m_operation{ Operation::None};
        int         m_previous_value{ 0};
        bool        m_dividedByZero{ false};
};

class CalculatorGui : public thWin32App
{
    public:
        CalculatorGui();

    private:
        CalculatorLogic             m_logic;

        std::unique_ptr< thForm>    m_MainWindow;
        
        std::unique_ptr< thEditBox> m_InputEditBox;

        std::unique_ptr< thButton>  m_Number0Button;
        std::unique_ptr< thButton>  m_Number1Button;
        std::unique_ptr< thButton>  m_Number2Button;
        std::unique_ptr< thButton>  m_Number3Button;
        std::unique_ptr< thButton>  m_Number4Button;
        std::unique_ptr< thButton>  m_Number5Button;
        std::unique_ptr< thButton>  m_Number6Button;
        std::unique_ptr< thButton>  m_Number7Button;
        std::unique_ptr< thButton>  m_Number8Button;
        std::unique_ptr< thButton>  m_Number9Button;
        
        std::unique_ptr< thButton>  m_PlusButton;
        std::unique_ptr< thButton>  m_MinusButton;
        std::unique_ptr< thButton>  m_MultiplyButton;
        std::unique_ptr< thButton>  m_DivisionButton;

        std::unique_ptr< thButton>  m_ResultButton;
        std::unique_ptr< thButton>  m_ClearButton;
        std::unique_ptr< thButton>  m_DeleteButton;

        // Generalized button onClick event callback.
        thResult_t Button_onClick( thObject *, thEventParams_t);

        // Wrap stoi exceptions.
        bool textToInt( int & a_result)
        {
            try
            {
                a_result = std::stoi( m_InputEditBox->Text);
                return true;
            }
            catch(...)
            {
                m_InputEditBox->Text = L"Internal error";
            
                m_PlusButton->Disable();
                m_MinusButton->Disable();
                m_MultiplyButton->Disable();
                m_DivisionButton->Disable();
                m_ResultButton->Disable();
                m_DeleteButton->Disable();

                m_Number0Button->Disable();
                m_Number1Button->Disable();
                m_Number2Button->Disable();
                m_Number3Button->Disable();
                m_Number4Button->Disable();
                m_Number5Button->Disable();
                m_Number6Button->Disable();
                m_Number7Button->Disable();
                m_Number8Button->Disable();
                m_Number9Button->Disable();
            }

            return false;
        }
};

// Windows application entry point.
#if TH_DEBUG_LEVEL == 0
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
 int main()
#endif
{
    CalculatorGui calculator;

    return calculator.Run();
}

// On creation allocate all visual components.
CalculatorGui::CalculatorGui()
{
    // Create Main Window.
    {
        m_MainWindow = std::make_unique< thForm>();

        m_MainWindow->Text = L"Calculator example";
        m_MainWindow->Width = 395;
        m_MainWindow->Height = 300;
        m_MainWindow->Resizable = false;
        m_MainWindow->OnDestroy = []( thObject *, thEventParams_t)
            {
                constexpr const thResult_t quit_application = 1;
                return quit_application;
            };
    }

    // Create input edit box.
    {
        constexpr int x_position = 5;
        constexpr int y_position = 5;

        m_InputEditBox = std::make_unique< thEditBox>( m_MainWindow.get(), x_position, y_position);

        m_InputEditBox->Width = m_MainWindow->Width - 2 * x_position;
        m_InputEditBox->Height = 50;
        m_InputEditBox->Font.SetSize(30);
        m_InputEditBox->Disable();
        m_InputEditBox->Text = L"0";
    }
    

    // Helper lambda to create button window objects.
    auto createButton = [](
        CalculatorGui &                 a_CalculatorGui,
        std::unique_ptr< thButton> &    a_button,
        thString                        a_button_name,
        int                             a_x_button_position,
        int                             a_y_button_position,
        uint32_t                        a_button_font_size
        )
    {
        a_button = std::make_unique< thButton>( a_CalculatorGui.m_MainWindow.get(), a_x_button_position, a_y_button_position);

        a_button->Width = 75;
        a_button->Height = 50;
        a_button->Font.SetSize( a_button_font_size);
        a_button->Text = a_button_name;

        // Register callback on single and double click, since double click can interrupt single one when fast clicking.
        a_button->OnClick = std::bind( &CalculatorGui::Button_onClick, &a_CalculatorGui, std::placeholders::_1, std::placeholders::_2 );
        a_button->OnDoubleClicked = std::bind( &CalculatorGui::Button_onClick, &a_CalculatorGui, std::placeholders::_1, std::placeholders::_2 );
    };

    // Create Button 7.
    {
        const auto x_position = 5;
        const auto y_position = 60;
        const auto font_size =  15;

        createButton( *this, m_Number7Button, L"7", x_position, y_position, font_size);
    }

    // Create Button 8.
    {
        const auto x_position = 5 + m_Number7Button->Width;
        const auto y_position = 60;
        const auto font_size =  15;

        createButton( *this, m_Number8Button, L"8", x_position, y_position, font_size);
    }
    
    // Create Button 9.
    {
        const auto x_position = 5 + m_Number8Button->Width + m_Number7Button->Width;
        const auto y_position = 60;
        const auto font_size =  15;

        createButton( *this, m_Number9Button, L"9", x_position, y_position, font_size);
    }
    
    // Create Button 4.
    {
        const auto x_position = 5;
        const auto y_position = 60 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number4Button, L"4", x_position, y_position, font_size);
    }
    
    // Create Button 5.
    {
        const auto x_position = 5 + m_Number4Button->Width;
        const auto y_position = 60 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number5Button, L"5", x_position, y_position, font_size);
    }
    
    // Create Button 6.
    {
        const auto x_position = 5 + m_Number4Button->Width + m_Number5Button->Width;
        const auto y_position = 60 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number6Button, L"6", x_position, y_position, font_size);
    }

    // Create Button 1.
    {
        const auto x_position = 5;
        const auto y_position = 60 + 50 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number1Button, L"1", x_position, y_position, font_size);
    }
    
    // Create Button 2.
    {
        const auto x_position = 5 + m_Number4Button->Width;
        const auto y_position = 60 + 50 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number2Button, L"2", x_position, y_position, font_size);
    }
    
    // Create Button 3.
    {
        const auto x_position = 5 + m_Number4Button->Width + m_Number5Button->Width;
        const auto y_position = 60 + 50 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number3Button, L"3", x_position, y_position, font_size);
    }

    // Create Button 0.
    {
        const auto x_position = 5 + m_Number4Button->Width;
        const auto y_position = 60 + 50 + 50 + 50;
        const auto font_size =  15;

        createButton( *this, m_Number0Button, L"0", x_position, y_position, font_size);
    }
    
    // Create plus Button.
    {
        const auto x_position = 15 + 3* m_Number4Button->Width;
        const auto y_position = 60;
        const auto font_size =  20;

        createButton( *this, m_PlusButton, L"+", x_position, y_position, font_size);
    }

    // Create minus Button.
    {
        const auto x_position = 15 + 3* m_Number4Button->Width;
        const auto y_position = 60 + 50;
        const auto font_size =  20;

        createButton( *this, m_MinusButton, L"-", x_position, y_position, font_size);
    }

    // Create multiply Button.
    {
        const auto x_position = 15 + 3* m_Number4Button->Width;
        const auto y_position = 60 + 50 + 50;
        const auto font_size =  20;

        createButton( *this, m_MultiplyButton, L"x", x_position, y_position, font_size);
    }

    // Create division Button.
    {
        const auto x_position = 15 + 3* m_Number4Button->Width;
        const auto y_position = 60 + 50 + 50 + 50;
        const auto font_size =  20;

        createButton( *this, m_DivisionButton, L"÷", x_position, y_position, font_size);
    }

    // Create clear Button.
    {
        const auto x_position = 15 + 4* m_Number4Button->Width;
        const auto y_position = 60;
        const auto font_size =  20;

        createButton( *this, m_ClearButton, L"CLR", x_position, y_position, font_size);
    }

    // Create minus Button.
    {
        const auto x_position = 15 + 4* m_Number4Button->Width;
        const auto y_position = 60 + 50;
        const auto font_size =  20;

        createButton( *this, m_DeleteButton, L"DEL", x_position, y_position, font_size);
    }

    // Create result Button.
    {
        const auto x_position = 15 + 4* m_Number4Button->Width;
        const auto y_position = 60 + 50 + 50 + 50;
        const auto font_size =  20;

        createButton( *this, m_ResultButton, L"=", x_position, y_position, font_size);
    }

    m_MainWindow->Show(); // Show main window.
}

thResult_t CalculatorGui::Button_onClick( thObject * sender, thEventParams_t)
{
    thButton * pressed_button = dynamic_cast< thButton*> ( sender);

    const size_t number_of_buttons{ 10U};

    // Group together number buttons.
    const std::array< const thButton *,number_of_buttons> number_buttons
    {
        m_Number0Button.get(), m_Number1Button.get(), m_Number2Button.get(),
        m_Number3Button.get(), m_Number4Button.get(), m_Number5Button.get(),
        m_Number6Button.get(), m_Number7Button.get(), m_Number8Button.get(),
        m_Number9Button.get()
    };

    // Check number button first.
    for ( size_t i = 0; i < number_of_buttons; ++i)
    {
        if ( number_buttons[ i] == pressed_button)
        {
            if ( m_logic.isDivByZero())
            {
                m_logic.clear();

                m_InputEditBox->Text = std::to_wstring( i);

                m_PlusButton->Enable();
                m_MinusButton->Enable();
                m_MultiplyButton->Enable();
                m_DivisionButton->Enable();
                m_ResultButton->Enable();
                m_DeleteButton->Enable();
            }
            else
            {
                std::wstring current_text = m_InputEditBox->Text;

                if ( !current_text.compare( L"0"))
                {
                    m_InputEditBox->Text = std::to_wstring( i);
                }
                else
                {
                    m_InputEditBox->Text += std::to_wstring( i);
                }
            }

            return 1;
        }
    }

    // Check functional buttons.

    // Plus
    if ( m_PlusButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.add( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Minus
    if ( m_MinusButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.minus( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Multiply
    if ( m_MultiplyButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.multiply( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Division
    if ( m_DivisionButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.divide( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Result
    if ( m_ResultButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        int result = 0;

        if ( true == m_logic.result( current_value, result))
        {
            m_InputEditBox->Text = std::to_wstring( result);
        }
        else
        {
            m_InputEditBox->Text = L"Division by zero";
            
            m_PlusButton->Disable();
            m_MinusButton->Disable();
            m_MultiplyButton->Disable();
            m_DivisionButton->Disable();
            m_ResultButton->Disable();
            m_DeleteButton->Disable();
        }

        return 1;
    }

    // Clear
    if ( m_ClearButton.get() == pressed_button)
    {
        m_logic.clear();
        m_InputEditBox->Text = L"0";

        m_PlusButton->Enable();
        m_MinusButton->Enable();
        m_MultiplyButton->Enable();
        m_DivisionButton->Enable();
        m_ResultButton->Enable();
        m_DeleteButton->Enable();

        m_Number0Button->Enable();
        m_Number1Button->Enable();
        m_Number2Button->Enable();
        m_Number3Button->Enable();
        m_Number4Button->Enable();
        m_Number5Button->Enable();
        m_Number6Button->Enable();
        m_Number7Button->Enable();
        m_Number8Button->Enable();
        m_Number9Button->Enable();

        return 1;
    }

    // Delete
    if ( m_DeleteButton.get() == pressed_button)
    {
        std::wstring current_text = m_InputEditBox->Text;

        // If there is one digit left.
        if ( 1 == current_text.size())
        {
            m_InputEditBox->Text = L"0";
        }
        // If there are 2 digit left, check if
        // '-' sign is not first.
        if ( 2 == current_text.size())
        {
            if ( L'-' == current_text.at(0))
            {
                m_InputEditBox->Text = L"0";
            }
            else
            {
                current_text.erase( current_text.size() - 1);

                m_InputEditBox->Text = current_text;
            }
        }
        else if ( current_text.size() > 1)
        {
            current_text.erase( current_text.size() - 1);

            m_InputEditBox->Text = current_text;
        }

        return 1;
    }

    return 1;
}
