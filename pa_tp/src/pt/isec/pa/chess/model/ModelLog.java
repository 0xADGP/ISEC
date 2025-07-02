package pt.isec.pa.chess.model;


import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.List;
import java.beans.PropertyChangeSupport;

/**
 * Singleton logger class for game events.
 * Uses PropertyChangeSupport to notify listeners when new messages are logged.
 * Useful for debugging and UI message display.
 *
 * @author Antonio Pedorso, Luis Duarte, Carolina Veloso
 * @version 1.0.0
 */

public class ModelLog {
    private static ModelLog _instance = null;

    private final PropertyChangeSupport pcs;
    public static final String PROP_NEW_LOG = "newLog";

    protected ArrayList<String> log;

    /**
     * Private constructor for the ModelLog singleton.
     * Initializes the internal log list and property change support.
     */
    private ModelLog() {
        log = new ArrayList<>();
        pcs = new PropertyChangeSupport(this);
    }

    /**
     * Returns the singleton instance of the logger.
     *
     * @return ModelLog instance
     */
    public static ModelLog getInstance() {
        if (_instance == null) _instance = new ModelLog();
        return _instance;
    }

    /**
     * Clears all log messages.
     */
    public void reset() {
        log.clear();
    }

    /**
     * Adds a message to the log and notifies listeners.
     *
     * @param msg The message to log
     */
    public void log(String msg) {
        log.add(msg);
        pcs.firePropertyChange(PROP_NEW_LOG, null, msg);
    }

    /**
     * Returns a copy of the current log messages.
     *
     * @return List of log strings
     */
    public List<String> getLog() {
        return new ArrayList<>(log);
    }

    /*
    public void addPropertyChangeListener(PropertyChangeListener listener) {
        pcs.addPropertyChangeListener(PROP_NEW_LOG, listener);
    }*/

    /**
     * Adds a listener to be notified of new log messages.
     *
     * @param propertyName Property to listen to
     * @param listener     The listener to notify
     */
    public void addPropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        pcs.addPropertyChangeListener(propertyName, listener);
    }

}