import copium;
import copium.window;

int main()
{
	copium::SayHi();
	window::Window window("window", 100, 100, 800, 600, {});
    window.open();
	while (!(window.closed))
    {
        window::windowControls.pollEvents();
        if (window.shouldClose())
        {
            window.close();
        }
        
    }
}