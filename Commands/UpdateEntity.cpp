#include "UpdateEntity.hpp"

namespace Commands {

    UpdateEntity::UpdateEntity()
    {

    }

    /**
     * @brief UpdateEntity::undo
     */
    void UpdateEntity::undoImpl()
    {
        sanityCheck();
    }

    /**
     * @brief UpdateEntity::redo
     */
    void UpdateEntity::redoImpl()
    {
        sanityCheck();

        if (!m_Done) {

        } else {

        }
    }

    /**
     * @brief UpdateEntity::sanityCheck
     */
    void UpdateEntity::sanityCheck()
    {

    }

    /**
     * @brief UpdateEntity::cleanup
     */
    void UpdateEntity::cleanup()
    {

    }

} // namespace Commands
